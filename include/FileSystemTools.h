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

    protected:

    private:
};

#endif // FILESYSTEMTOOLS_H
