#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <Serial.h>
#include <tools.h>
#include <FileSystemTools.h>

class fileSystem
{
    public:
        fileSystem();
        virtual ~fileSystem();

        SerialPort serial;
        FileSystemTools fst;

        void showDir();
        void changeDir(std::string dir);
        bool dirExists(const std::string& dirName_in);
        bool fileExists(const std::string& fileName);
        void printBigFile(std::string path);
        void read_dir_vect(const std::string& name, std::vector<std::string>& v);
        std::string getPath();
        int DeleteDirectory(const std::string &refcstrRootDirectory, bool bDeleteSubdirectories = true);
        void hideFile(const std::string& fileName);

        std::string defColor;
        std::string cd;

    private:
        std::string key;
        std::vector<int> findLocation(std::string sample, char findIt);
};

#endif // FILESYSTEM_H
