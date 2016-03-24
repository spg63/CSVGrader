//
//  StrUtils.cpp
//  cpplibs
//
//  Created by Sean Grimes on 11/26/15.
//  Copyright © 2015 Sean Grimes. All rights reserved.
//

#include <thread>
#include "StrUtils.hpp"

void StrUtils::toUpper(std::string &s){
    transform(s.begin(), s.end(), s.begin(), ::toupper);
}

void StrUtils::toLower(std::string &s){
    transform(s.begin(), s.end(), s.begin(), ::tolower);
}

std::string StrUtils::trim(const std::string &s){
    if(s.find_first_not_of("\t\n\a\b\f\r\v ") == std::string::npos)
        return "";
    
    std::string trimmed = s;
    // Removing tab chars
    trimmed.erase(remove(trimmed.begin(), trimmed.end(), '\t'), trimmed.end());
    // Removing new line chars
    trimmed.erase(remove(trimmed.begin(), trimmed.end(), '\n'), trimmed.end());
    // Removing 'audible bell'
    trimmed.erase(remove(trimmed.begin(), trimmed.end(), '\a'), trimmed.end());
    // Removing backspace
    trimmed.erase(remove(trimmed.begin(), trimmed.end(), '\b'), trimmed.end());
    // Removing form-feed
    trimmed.erase(remove(trimmed.begin(), trimmed.end(), '\f'), trimmed.end());
    // Removing carriage return
    trimmed.erase(remove(trimmed.begin(), trimmed.end(), '\r'), trimmed.end());
    // Removing vertical tab
    trimmed.erase(remove(trimmed.begin(), trimmed.end(), '\v'), trimmed.end());
    
    
    ///*************************************************************************
    // Addition for CSVGrader Program
    trimmed.erase(remove(trimmed.begin(), trimmed.end(), '\"'), trimmed.end());
    ///*************************************************************************
    
    auto first_char = trimmed.find_first_not_of(" ");
    auto last_char = trimmed.size() - first_char;
    std::string no_leading_ws = trimmed.substr(first_char, last_char);
    // Resetting first_char to 0;
    first_char = 0;
    last_char = no_leading_ws.find_last_not_of(" ");
    std::string all_trimmed = no_leading_ws.substr(first_char, last_char + 1);
    return all_trimmed;
}

std::vector<std::string> StrUtils::trimStringVector(const std::vector<std::string> &v){
    std::vector<std::string> r_vec(v.size());
    for(int i = 0; i < v.size(); ++i){
        r_vec[i] = trim(v[i]);
    }
    return r_vec;
}

std::vector<std::string> StrUtils::trimStrVec(
        const std::vector<std::string> &v) {
    auto num_threads = std::thread::hardware_concurrency();
    auto num_strs = v.size();
    auto num_strs_per_thread = num_strs / num_threads;

    // If there aren't at least 2 string per split, run single thread version
    if(num_strs_per_thread < 25)
        return trimStringVector(v);

    // Create num_threads amount of vectors
    std::vector<std::vector<std::string>> thread_vecs;
    for(auto i = 0; i < num_threads; ++i) {
        std::vector<std::string> vec;
        thread_vecs.push_back(vec);
    }

    // Add the strings to their respective vectors
    auto num_loops = num_strs_per_thread * num_threads;
    int vec_num = 0;
    for(auto i = 0; i < num_loops; ++i){
        thread_vecs[vec_num].push_back(v[i]);
        if(i % num_strs_per_thread == 0 && i != 0)
            ++vec_num;
    }

    // Add the remaining strings
    for(auto i = num_loops; i < num_strs; ++i){
        thread_vecs[vec_num].push_back(v[i]);
    }

    // Launch the threads
    std::vector<std::future<std::vector<std::string>>> futures;
    for(auto i = 0; i < num_threads; ++i){
        futures.push_back(std::async(std::launch::async, &StrUtils::trimStringVector,
                                     thread_vecs[i]));
    }

    // Get the results
    std::vector<std::vector<std::string>> results;
    for(auto i = 0; i < num_threads; ++i){
        results.push_back(futures[i].get());
    }

    // Put the results into a single vector
    std::vector<std::string> all_trimmed;
    for(auto &&vec : results){
        for(auto &&str : vec){
            all_trimmed.push_back(str);
        }
    }

    return all_trimmed;
}


std::vector<std::string> StrUtils::parseOnCharDelim(const std::string &line, const char delim){
    std::vector<std::string> vec;
    std::stringstream ss{line};
    std::string tmp;
    while(std::getline(ss, tmp, delim)){
        vec.push_back(tmp);
    }
    return vec;
}