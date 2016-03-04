//
//  main.cpp
//  CSVGrader
//
//  Created by Sean Grimes on 3/3/16.
//  Copyright © 2016 Sean Grimes. All rights reserved.
//

#include "CSVGrader.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <future>
#include "StrUtils.hpp"
#include "FileUtils.hpp"

const std::string RESULTS{"results.csv"};

void usage();
std::vector<std::string> find_users_from_bblearn_file(std::string codelabs_fp,
                                                      std::string bblearn_fp);
std::vector<std::string> get_users(std::string user_path);
void write_header(std::string problems_fp);

int main(int argc, const char * argv[]) {
   
    if(argc < 4)
        usage();
    
    std::vector<std::string> user_names;
    
    std::string codelabs{argv[1]};
    std::string problems{argv[2]};
    std::string users{argv[3]};
    if(argc == 4){ // -f list of user names has been provided, just read them in
        user_names = get_users(users);
    }
    else if(argc == 5){ // -f flag -- find users from bblearn.csv file
        user_names = find_users_from_bblearn_file(codelabs, users);
    }
    
    // Read in the user names
    auto num_users = user_names.size();
    
    // Easy threading in c++11+
    std::vector<std::future<std::string>> futures;
    
    /*
     * So here is the reason this is done with objects:
     * A thread for each user in the user list provided; separate objects allow
     * for multiple copies of the file in memory. While this uses more memory, 
     * the files are relatively small (all less than 1MB), processing is 
     * significantly faster (less cache misses, no file contention, no contention
     * for writing / printing results, and no need for locking)
     */
    for(auto &&user_name : user_names){
        CSVGrader cvs(codelabs, problems, user_name);
        futures.push_back(std::async(std::launch::async, &CSVGrader::generate_report_for_user, cvs));
    }
    
    // Explicit call to get() to ensure processing is done on all threads
    std::vector<std::string> results;
    results.reserve(num_users);
    for(auto i = 0; i < num_users; ++i){
        results.push_back(futures[i].get());
    }
    
    // Printing here instead of the above loop since output streams are not
    // thread safe by default
    std::ofstream results_fp;
    results_fp.open(RESULTS, std::ofstream::app);
    if(!results_fp) exit(-1000000);
    
    write_header(problems);
    for(auto i = 0; i < num_users; ++i){
        results_fp << results[i];
        //std::cout << results[i];
    }
    
    results_fp.close();
    
    return 0;
}

void write_header(std::string problems_fp){
    std::ofstream fp(RESULTS);
    auto probs = FileUtils::readLineByLine(problems_fp);
    fp << " " << ",";
    for(auto &&prob : probs)
        fp << prob << ",";
    
    fp << "\n";
}

std::vector<std::string> get_users(std::string user_path){
    std::ifstream in(user_path);
    if(!in){
        fprintf(stderr, "Couldn't open %s\n", user_path.c_str());
        exit(-1);
    }
    
    std::vector<std::string> users;
    std::string user;
    while(std::getline(in, user))
        users.emplace_back(StrUtils::trim(user));
    
    return users;
}

std::vector<std::string> find_users_from_bblearn_file(std::string codelabs_fp,
                                                      std::string bblearn_fp){
    
    std::string fname;
    std::string lname;
    int bblearn_fname_col = 1;
    int bblearn_lname_col = 0;
    int codelab_fname_col = 5;
    int codelab_lname_col = 4;
    int codelab_first_useful_row = 3;
    int codelab_email_col = 6;
    int bblearn_first_useful_row = 1;
    std::vector<std::string> user_list;
    
    // Read the files into memory
    auto codelabs_lines = FileUtils::readLineByLine(codelabs_fp);
    auto bblearn_lines = FileUtils::readLineByLine(bblearn_fp);
    
    // Turn them into matricies
    std::vector<std::vector<std::string>> codelabs_mat;
    std::vector<std::vector<std::string>> bblearn_mat;
    
    for(auto &&line : codelabs_lines)
        codelabs_mat.push_back(StrUtils::parseOnCharDelim(line, ','));
    
    for(auto &&line : bblearn_lines)
        bblearn_mat.push_back(StrUtils::parseOnCharDelim(line, ','));
    
    std::vector<std::string> bblearn_lnames;
    std::vector<std::string> bblearn_fnames;
    
    // Parallel arrays of first name and last name from bblearn csv file
    for(auto i = bblearn_first_useful_row; i < bblearn_mat.size(); ++i){
        bblearn_lnames.push_back(StrUtils::trim(bblearn_mat[i][bblearn_lname_col]));
        bblearn_fnames.push_back(StrUtils::trim(bblearn_mat[i][bblearn_fname_col]));
    }
    
    // Now search the codelabs csv file for the names and get the emails
    for(auto i = 0; i < bblearn_lnames.size(); ++i){
        for(auto j = codelab_first_useful_row; j < codelabs_mat.size(); ++j){
            // Check if we have a matching last name
            if(StrUtils::trim(codelabs_mat[j][codelab_lname_col]).find(bblearn_lnames[i]) != std::string::npos){
                if(StrUtils::trim(codelabs_mat[j][codelab_fname_col]).find(bblearn_fnames[i]) != std::string::npos){
                    // We have matching first and last names, add the email to the user IDs
                    user_list.push_back(codelabs_mat[j][codelab_email_col]);
                }
                // NOTE: No breaking, some users are registered twice (hence the need for this search)
            }
        }
    }
    return user_list;
}

void usage(){
    std::cout << "./CSVGrader codelabs.csv problem_list.csv {user_list.{csv txt} OR bblearn.csv} [-f] \n\n";
    std::cout << "If '-f' flag, provide the bblearn.csv file instead of the\n";
    std::cout << "manually written user list file and the program will find\n";
    std::cout << "the codelab usernames by name matching\n";
    std::cout << "\nNOTE: If users are registered multiple time, results for\n";
    std::cout << "each registration will be provided, typically one right\n";
    std::cout << "after the other.\n\n";
    exit(0);
}