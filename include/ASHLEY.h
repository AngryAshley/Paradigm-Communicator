#ifndef ASHLEY_H
#define ASHLEY_H

#include <future>
#include <chrono>
#include <windows.h>
#include <signal.h>
#include <tools.h>
#include <Serial.h>
#include <FileSystemTools.h>
//#include <thread>


class ASHLEY
{
    public:
        ASHLEY();
        virtual ~ASHLEY();

        SerialPort serial;
        Tools tools;
        FileSystemTools fstools;

        void controlPanel();

    protected:

    private:
        void printBorders();
        bool running = true;
        std::string waitForKey();
};

#endif // ASHLEY_H
