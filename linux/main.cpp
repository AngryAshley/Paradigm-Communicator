
#include <iostream>     // File Descriptor handling definitions
#include <stdlib.h>     // Standard Library definitions
#include <string.h>     // string function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <unistd.h>     // UNIX standard function definitions
#include <thread>       // Multithreading definitions

#include "serial.h"

using namespace std;

string PCS_ver="0.1.0 A";
string newLine = "\r";
Serial* serialConection;

void hexdump(string str) {
    static const char* const lut = "0123456789ABCDEF";
    size_t len = str.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = str[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    cout << output << endl;
}

void STDInToSerial (bool* shouldStop) {
    // parse a boolean POINTER to stop outside thread scope
    while (!*shouldStop) {
        // reset buffer to null byte
        char buf = '\0';
        // define a new filedescriptor set
        fd_set set;
        // set some params
        FD_ZERO(&set);
        FD_SET(0, &set);
        // define timeout
        struct timeval timeout;
        // set some params
        timeout.tv_sec = 0;
        timeout.tv_usec = 10000;
        // use select() to check if there's data to read
        int rv = select(1, &set, NULL, NULL, &timeout);
        // if data, read it, else loop
        if (rv > 0)
            read(0, &buf, 1);
        if ((char)buf == (char)'\r' || (char)buf == (char)'\n' || (char)buf == (char)'\0')
            continue;
        serialConection->writeData((string)&buf);
    }
}

void readConfig () {}

bool checkLogin(string username, string password) {
    if (username == (string)"dan\r" && password == (string)"pass\r")
        return true;
    return false;
}

bool login() {
    serialConection->writeData((string)"Username: ");
    string username = serialConection->readData(false, '\0');
    serialConection->writeData(newLine);
    serialConection->writeData((string)"Password: ");
    string password = serialConection->readData(true, '\0');
    serialConection->writeData(newLine);
    return checkLogin(username, password);
}

void welcome() {
    serialConection->writeData((string)"Welcome user!");
    serialConection->writeData(newLine);
}

void runProgram() {
    serialConection->writeData("Paradigm Communicator " + PCS_ver);
    serialConection->writeData(newLine);
    if (login())
        welcome();
}

int main() {
    // make a new Serial object
    serialConection = new Serial("/dev/pts/2");
    // read config (nothing right now)
    readConfig();
    // set shouldStop (for the loops in threads)
    bool shouldStop = false;
    // start program thread
    thread x(runProgram);
    // start STDIN reading thread, with the shouldStop var
    thread y(STDInToSerial, &shouldStop);

    // wait for main program to finish
    x.join();
    // set the shouldStop var to true, stops other threads
    shouldStop = true;
    // wait for other threads to stop
    y.join();
    // return no error
    return 0;
}
