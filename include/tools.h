#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <locale>
#include <fstream>


class Tools
{
    public:
        Tools();
        virtual ~Tools();

        void splitString(std::string temp, std::string output[], char* tokens = " ,.-");
        std::string to_upper(std::string input);
        std::string date();
        int dateToInt(int day, int month, int year, int hour, int minute);
        bool compareArray( const std::array<int, 2>& u, const std::array<int, 2>& v );
        int auth(std::string user);
        std::string setting_read(std::string setting, std::string path);
        int throwError(int type, std::string info1, int severity=0);

        std::string pathexe;
};

#endif // TOOLS_H
