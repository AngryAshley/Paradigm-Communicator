#ifndef SERIALTOOLS_H
#define SERIALTOOLS_H

#include <Serial.h>
#include <iostream>
#include <string>


class serialTools
{
    public:
        serialTools();
        virtual ~serialTools();

        SerialPort serial;
        int choice(std::string prompt, std::string choices="yn");


    protected:

    private:
};

#endif // SERIALTOOLS_H
