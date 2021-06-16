#ifndef SAS_H
#define SAS_H

#include <tools.h>
#include <fileSystem.h>
#include <FileSystemTools.h>
#include <Serial.h>
#include <serialTools.h>
#include <vector>
#include <string>

class SAS
{
    public:
        SAS();
        virtual ~SAS();
        FileSystemTools fst;
        fileSystem fs;
        Tools tools;
        SerialPort serial;
        serialTools sTools;

        std::string path_exe;
        void menu(int clearance, int archivist, std::string uname);
        int check(bool verbose=true);
        void inbox();
};

#endif // SAS_H
