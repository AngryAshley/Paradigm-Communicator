#include "tools.h"

Tools::Tools()
{
    //ctor
}

Tools::~Tools()
{
    //dtor
}

void Tools::splitString(std::string temp, std::string output[], char* tokens){
    //printf("Splitstring Called");
    int s=0;
    char str[255];
    strcpy(str, temp.c_str());
    //printf(" - copy successful\n");
    char * pch;
    pch = strtok (str,tokens);
    while (pch != NULL){
        output[s] = pch;
        pch = strtok (NULL, tokens);
        s++;
    }
}

std::string Tools::to_upper(std::string input){
    std::locale loc;
    std::string temp;
    for (std::string::size_type i=0; i<input.length(); ++i)
        temp += std::toupper(input[i],loc);
    return temp;
}

std::string Tools::date(){
    time_t now = time(0);
    char* dt = ctime(&now);
    return dt;
}

int Tools::dateToInt(int day, int month, int year){
    return year*10000 + month*100 + day;
}

bool Tools::compareArray( const std::array<int, 2>& u, const std::array<int, 2>& v ){
    return u[1] < v[1];
};
