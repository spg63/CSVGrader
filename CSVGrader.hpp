//
//  CSVGrader.hpp
//  CSVGrader
//
//  Created by Sean Grimes on 3/3/16.
//  Copyright © 2016 Sean Grimes. All rights reserved.
//

#ifndef CSVGrader_hpp
#define CSVGrader_hpp

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "StrUtils.hpp"
#include "FileUtils.hpp"

class CSVGrader{
public:
    /**
     * Path to the codelabs csv report
     * Path to the list of problems, don't care if csv or txt;
     *  NOTE: Problems need to be newline delineated
     * The user ID as it appears in codelabs, full email okay but not necessary
     */
    CSVGrader(const std::string &codelab_path,
              const std::string &prob_list_path,
              const std::string &codelab_userID = "",
              const std::string &bblearn_csv_path = "");
    
    std::string generate_report_for_user();
   
private:
    void read_files_into_memory();
    void spliton(const std::vector<std::string> &input,
                 std::vector<std::vector<std::string>> &output,
                 const char delim);
    void find_user_row_num();
    void check_all_problems();
    void build_report_string();
    
private:
    const std::string m_PREPEND = "00000-";
    const std::string m_clabs_email_header = "Email";
    const char m_comma = ',';
    size_t m_code_labs_user_row{};
    std::string m_report;
    std::vector<std::string> m_report_v;
    bool m_search_bb_learn{false};
    
    
    std::string m_user_id;
    std::string m_bblearn_fp;
    std::string m_codelab_fp;
    std::string m_problems_fp;
    std::string m_alternate_id_fp;
    std::vector<std::string> m_labs_lines;
    std::vector<std::string> m_bblearn_lines;
    std::vector<std::string> m_problem_lines;
    std::vector<std::string> m_alternate_id_lines;
    std::vector<std::vector<std::string>> m_labs_matrix;
    
};

    /*
      If the bblearn file path is provided, find the codelab user names
      by using fname/lname from bblearn and matching usernames since
      some students have a non-drexel email for codelabs
      NOTE: This is optional, can provide a username at startup and
     */
    std::vector<std::string> find_all_users();

#endif /* CSVGrader_hpp */
