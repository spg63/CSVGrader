//
//  FileUtils.hpp
//  cpplibs
//
//  Created by Sean Grimes on 12/7/15.
//  Copyright © 2015 Sean Grimes. All rights reserved.
//

#ifndef FileUtils_hpp
#define FileUtils_hpp
#include <string>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <cstring>
#include <unistd.h>
#include "StrUtils.hpp"

class FileUtils{
public:
    static std::string readFullFile(const std::string &fileName);
    static std::vector<std::string> readLineByLine(const std::string &fileName);
    static size_t lineCount(const char *fname);
    static std::vector<std::string> getFilesInDir(const std::string &dirPath, std::string ext = "");
    static std::vector<std::string> getDirsInDir(const std::string &path);
    static bool isFile(const std::string &path);
    static bool isDir(const std::string &path);
    static bool ends_with_string(const std::string &str, const std::string &what);
};



#endif /* FileUtils_hpp */
