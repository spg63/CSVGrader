//
//  FileUtils.cpp
//  cpplibs
//
//  Created by Sean Grimes on 12/7/15.
//  Copyright © 2015 Sean Grimes. All rights reserved.
//

#include "FileUtils.hpp"

std::string FileUtils::readFullFile(const std::string &fileName){
    std::ifstream ifs(fileName.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    if(!ifs.is_open()){
        throw std::runtime_error("Couldn't open " + fileName);
    }
    std::ifstream::pos_type f_size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    
    std::vector<char> bytes(f_size);
    ifs.read(&bytes[0], f_size);

    return std::string(&bytes[0], f_size);
}

std::vector<std::string> FileUtils::readLineByLine(const std::string &fileName){
    std::string line;
    std::ifstream in(fileName);
    if(!in.is_open()){
        throw std::runtime_error("Couldn't open " + fileName);
    }
    std::vector<std::string> vec;
    vec.reserve(lineCount(fileName.c_str()));
    while(std::getline(in, line)){
        vec.push_back(StrUtils::trim(line));
    }
    vec.shrink_to_fit();
    return vec;
}

// NOTE: This is more or less taken directly from the "wc" command line utility
size_t FileUtils::lineCount(const char *fname){
    static const auto BUF_SIZE = 16*1024;
    int fd = open(fname, O_RDONLY);
    if(fd == -1)
        throw std::runtime_error("Couldn't open " + std::string(fname));
    
    char buf[BUF_SIZE + 1];
    size_t lines = 0;
    
    while(size_t bytes_read = read(fd, buf, BUF_SIZE)){
        if(bytes_read == (size_t) -1)
            throw std::runtime_error("Read Failure" + std::string(fname));
        if(!bytes_read)
            break;
        
        for(char *p = buf; (p = (char *) memchr(p, '\n', (buf + bytes_read) - p)); ++p){
            ++lines;
        }
    }
    close(fd);
    return lines;
}

std::vector<std::string> FileUtils::getFilesInDir(const std::string &dirPath, std::string ext){
    std::vector<std::string> files;
    
    DIR *dir;
    struct dirent *ent;
    
    if ((dir = opendir(dirPath.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if(isFile(std::string(dirPath + "/" + ent->d_name))){
                if(ext != ""){
                    // Note: This can easily be fooled
                    std::string f_name(ent->d_name);
                    if(ends_with_string(f_name, ext)){
                        files.push_back(f_name);
                    }
                }
                // Don't care what the file extension is
                else{
                    files.emplace_back(ent->d_name);
                }
            }
        }
        closedir(dir);
    }
    else {
        // could not open directory
        throw std::runtime_error("Could not open dir " + dirPath);
    }
    
    return files;
}

std::vector<std::string> FileUtils::getDirsInDir(const std::string &path){
    std::vector<std::string> dirs;
    DIR *dir;
    struct dirent *ent;
    if((dir = opendir(path.c_str())) != NULL){
        while((ent = readdir(dir)) != NULL){
            if(isDir(std::string(path + "/" + ent->d_name))){
                dirs.emplace_back(ent->d_name);
            }
        }
    }
    return dirs;
}

bool FileUtils::isFile(const std::string &path){
    struct stat buffer;
    stat(path.c_str(), &buffer);
    return S_ISREG(buffer.st_mode);
}

bool FileUtils::isDir(const std::string &path){
    struct stat buffer;
    stat(path.c_str(), &buffer);
    return S_ISDIR(buffer.st_mode);
}

bool FileUtils::ends_with_string(const std::string &str, const std::string &what) {
    return what.size() <= str.size() && str.find(what, str.size() - what.size()) != str.npos;
}




