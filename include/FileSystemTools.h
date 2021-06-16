#ifndef FILESYSTEMTOOLS_H
#define FILESYSTEMTOOLS_H

#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>

class FileSystemTools
{
    public:
        FileSystemTools();
        virtual ~FileSystemTools();

        std::string file_getLine(std::string path, int lineNumber);
        std::vector<std::string> file_getAll(std::string path);
        int file_writeAll(std::string path, std::vector<std::string> contents, bool truncate=true);
        void file_append(std::string path, std::string contents);

    protected:

    private:
};

#endif // FILESYSTEMTOOLS_H
