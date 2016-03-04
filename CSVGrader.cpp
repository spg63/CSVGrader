//
//  CSVGrader.cpp
//  CSVGrader
//
//  Created by Sean Grimes on 3/3/16.
//  Copyright © 2016 Sean Grimes. All rights reserved.
//

#include "CSVGrader.hpp"

CSVGrader::CSVGrader(const std::string &codelab_path,
                     const std::string &prob_list_path,
                     const std::string &codelab_userID,
                     const std::string &bblearn_csv_path)
    : m_codelab_fp(codelab_path)
    , m_problems_fp(prob_list_path)
    , m_bblearn_fp(bblearn_csv_path)
    , m_user_id(codelab_userID)
{
    if(bblearn_csv_path.length())
        m_search_bb_learn = true;
}

std::string CSVGrader::generate_report_for_user(){
    // Read the codelab csv and problem list into memory
    read_files_into_memory();
    
    // Split the codelab csv into a matrix of sorts
    spliton(m_labs_lines, m_labs_matrix, m_comma);
    
    // Determine the row in the codelab csv for current userID
    find_user_row_num();
    
    check_all_problems();
    
    build_report_string();
    
    return m_report;
}

void CSVGrader::read_files_into_memory(){
    m_problem_lines = FileUtils::readLineByLine(m_problems_fp);
    m_labs_lines = FileUtils::readLineByLine(m_codelab_fp);
}

void CSVGrader::spliton(const std::vector<std::string> &input,
                        std::vector<std::vector<std::string>> &output,
                        const char delim){
    for(auto &&line : input)
        output.push_back(StrUtils::parseOnCharDelim(line, delim));
}

void CSVGrader::find_user_row_num(){
    size_t num_rows{m_labs_matrix.size()};
    size_t user_id_col{0};
    
    // Find out which column the user id is held in, search the top row
    for(auto &&header : m_labs_matrix[0]){
        if(header.find(m_clabs_email_header) == std::string::npos)
            ++user_id_col;
        else
            break;
    }
    
    // Find out which row the user in on
    for(auto i = 0; i < num_rows; ++i){
        if(m_labs_matrix[i][user_id_col].find(m_user_id) != std::string::npos){
            m_code_labs_user_row = i;
            return;
        }
    }
}

void CSVGrader::check_all_problems(){
    m_report_v.push_back(m_user_id + ",");
    
    size_t col_num{0};
    for(auto &&prob_num : m_problem_lines){
        for(int i = 0; i < m_labs_matrix[0].size(); ++i){
            if(m_labs_matrix[0][i].find(prob_num) != std::string::npos){
                col_num = i;
                break;
            }
        }
        if(col_num != 0){
            m_report_v.push_back(m_labs_matrix[m_code_labs_user_row][col_num]);
            m_report_v.emplace_back(",");
        }
    }
    m_report_v.emplace_back("\n");
}

void CSVGrader::build_report_string(){
    for(auto &&str : m_report_v)
        m_report += str;
}





















