//
//  StrUtils.cpp
//  cpplibs
//
//  Created by Sean Grimes on 11/26/15.
//  Copyright © 2015 Sean Grimes. All rights reserved.
//

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

std::vector<std::string> StrUtils::parseOnCharDelim(const std::string &line, const char delim){
    std::vector<std::string> vec;
    std::stringstream ss{line};
    std::string tmp;
    while(std::getline(ss, tmp, delim)){
        vec.push_back(tmp);
    }
    return vec;
}