#ifndef ASHLEY_H
#define ASHLEY_H

#include <future>
#include <chrono>
#include <windows.h>
#include <signal.h>
#include <tools.h>
#include <Serial.h>
#include <FileSystemTools.h>
#include <thread>
#include <vector>
#include <conio.h>
#include <iostream>



class ASHLEY
{
    public:
        ASHLEY();
        virtual ~ASHLEY();

        SerialPort serial;
        Tools tools;
        FileSystemTools fstools;

        int ashleyBoot();
        void controlPanel();
        void tapeLeak();
        void printCanvas(int i); ///PUT BACK IN PRIVATE AFTER TESTING RADAR

    protected:

    private:

        void tapeLeakLog();
        bool running = true;
        bool terminate = false;
        std::string waitForKey();
        bool waitForMaster();
        std::vector<std::string> log;
        int second=0;
};

#endif // ASHLEY_H
