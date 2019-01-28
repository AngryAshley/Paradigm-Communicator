#include "FileSystemTools.h"

FileSystemTools::FileSystemTools()
{
    //ctor
}

FileSystemTools::~FileSystemTools()
{
    //dtor
}

std::string FileSystemTools::file_getLine(std::string path, int lineNumber){
    std::ifstream file(path);
    std::string temp;
    if(file.good()){
        for(int i=0; i<=lineNumber; i++){
            getline(file,temp);
        }
    file.close();
    }
    return temp;
}

std::vector<std::string> FileSystemTools::file_getAll(std::string path){
    std::vector<std::string> contents;
    std::ifstream file(path);
    std::string temp;
    if(file.good()){
        while (getline(file, temp)){
            contents.push_back(temp);
        }
    file.close();
    }
    return contents;
}
