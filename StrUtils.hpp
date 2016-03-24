//
//  StrUtils.hpp
//  cpplibs
//
//  Created by Sean Grimes on 11/26/15.
//  Copyright © 2015 Sean Grimes. All rights reserved.
//

#ifndef StrUtils_hpp
#define StrUtils_hpp

#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <thread>
#include <future>
#include <iostream>

class StrUtils{
public:
    
    /**
     * Recursively removes a specific character from a string
     *	First converts the char/digit to a string w/ sstream then searches the
     *		string for the char and removes the matching chars
     *
     * NOTE: This is case sensitive, ie 'S' != 's'
     *
     * @param: s- The string to remove the character from
     * @param: T - The char/int/double/float/string to remove
     * @return: std::string - A new string with the specific char removed
     */
    template<typename T>
    static std::string removeLetterFromString(std::string &s, T &ch){
        std::stringstream T_to_string;
        std::string s_ch;
        T_to_string << ch;
        T_to_string >> s_ch;
        
        if(s.length() == 0)
            return "";
        if(s.length() == 1){
            if(s_ch == s)
                return "";
            return s;
        }
        
        auto end = s.length() - 1;
        std::string test_s = s.substr(0, 1);
        std::string rest = s.substr(1, end);
        if(test_s == s_ch)
            return removeLetterFromString(rest, ch);
        return test_s + removeLetterFromString(rest, ch);
    }
    
    /**
     * Takes a string reference and uses std::transform to convert all
     *  chars to uppercase.
     *
     * @param: s - User passed string to be converted to uppercase
     * @return: void, works on pass by reference, avoid constructor
     *  calls, returns
     */
    static void toUpper(std::string &s);
    
    /**
     * Takes a string reference and uses std::transform to convert all
     *  chars to lowercase.
     *
     * @param: s - User passed string to be converted to lowercase
     * @return: void, works on pass by reference, avoid constructor
     *  calls, returns
     */
    static void toLower(std::string &s);
   
    /**
     * Trims the leading and trailing whitespace
     *  The function trims any blank space and non printing characters
     *  Done by stripping non printing characters with erase/remove idiom
     *      - Algorithm lib: remove --> make sure the erased chars aren't blank
     *      - String method: erase --> erase characters
     *
     *  Blankspace removed with find_first_not_of, find_last_not_of
     *
     * @param: s - User passed string to be trimmed
     * @return: std::string - Returns a trimmed/stripped string
     */
    static std::string trim(const std::string &s);

   /**
    * Depending on the length of your strings, the number of strings in the vector, and your
    * system, this may provide a significant speedup over calling the trim() function
    * individually on each string.
    *
    * The runtime for trim() can basically boil down to O(n*m) where n is the number of strings
    * and m is the length of your strings. Running trim() in parallel can give a significant
    * execution time advantage.
    *
    * NOTE: If vector isn't sufficiently large (testing shows 25 strings per thread min) this
    * will default to single thread operations; otherwise the vector will be split and
    * operated on concurrently. Splitting the vector is a sub-optimal solution, however
    * it's quick to implement.
    *  - Better ways to accomplish this are very welcome.
    */
    static std::vector<std::string> trimStrVec(const std::vector<std::string> &v);
    
    /**
     * Converts an int, double, or float to a string, returns the string
     *      Using strigstream
     *
     * @param: T a - an int, double, or floar
     * @return: std::string - the converted number
     */
    template<typename T>
    static std::string numToString(const T &a){
        std::stringstream vert;
        std::string b;
        vert << a;
        vert >> b;
        return b;
    }
    
    static std::vector<std::string> parseOnCharDelim(const std::string &line, const char delim);

private:
 /**
  * Trims each line of a vector<string> using the above trim function and
  *  returns a new vector of trimmed items
  *
  * It would be more efficient to use trim and a loop or iterator, but
  * sometimes a new vector of strings is preferable
  *
  * @param: v - The vector of strings to be trimmed
  * @return: std::vector<std::string> - The vector of trimmed strings
  */
    static std::vector<std::string> trimStringVector(const std::vector<std::string> &v);

};

#endif /* StrUtils_hpp */















