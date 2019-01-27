#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <locale>


class Tools
{
    public:
        Tools();
        virtual ~Tools();

        void splitString(std::string temp, std::string output[], char* tokens = " ,.-");
        std::string to_upper(std::string input);
        std::string date();
        int dateToInt(int day, int month, int year);
        bool compareArray( const std::array<int, 2>& u, const std::array<int, 2>& v );
};

#endif // TOOLS_H
