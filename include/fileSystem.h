#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <Serial.h>
#include <tools.h>
#include <FileSystemTools.h>
#include <serialTools.h>

class fileSystem
{
    public:
        fileSystem();
        virtual ~fileSystem();

        SerialPort serial;
        FileSystemTools fst;
        serialTools stools;
        Tools tools;

        void showDir();
        void changeDir(std::string dir);
        bool dirExists(const std::string& dirName_in);
        bool fileExists(const std::string& fileName);
        void printBigFile(std::string path);
        void read_dir_vect(const std::string& name, std::vector<std::string>& v);
        std::string getPath();
        int DeleteDirectory(const std::string &refcstrRootDirectory, bool bDeleteSubdirectories = true);
        void hideFile(const std::string& fileName);

        void db(std::string dbID, int clearance);
        int fileEditor(std::string path);

        std::string defColor;
        std::string cd;

    private:
        std::string key;
        std::vector<int> findLocation(std::string sample, char findIt);
        int fileEditor_drawMenu(int mode=0,int select1=0,int select2=0);
        std::string fe_currentFile;
};

#endif // FILESYSTEM_H
