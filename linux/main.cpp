
#include <iostream>     // File Descriptor handling definitions
#include <stdlib.h>     // Standard Library definitions
#include <string.h>     // string function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <unistd.h>     // UNIX standard function definitions
#include <thread>       // Multithreading definitions

#include "serial.h"

using namespace std;

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
    while (!*shouldStop) {
        char buf = '\0';
        read(0, &buf, 1);
        if ((char)buf == (char)'\r' || (char)buf == (char)'\n')
            continue;
        //usleep(timeout);
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
    serialConection->writeData((string)"Welcome to Paradigm Communicator!");
    serialConection->writeData(newLine);
    serialConection->writeData((string)"Username: ");
    string username = serialConection->readData(false, '\0');
    hexdump(username); //cout <<  username << endl;
    serialConection->writeData(newLine);
    serialConection->writeData((string)"Password: ");
    string password = serialConection->readData(true, '\0'); // stop bitching about this little thing, it works, shut up already
    serialConection->writeData(newLine);
    hexdump(password); //cout << password << endl;
    return checkLogin(username, password);
}

void welcome() {
    serialConection->writeData((string)"Welcome user!");
    serialConection->writeData(newLine);
}

void runProgram() {
    if (login())
        welcome();
}

int main() {
    serialConection = new Serial();
    serialConection->initConnection();
    readConfig();
    bool shouldStop = false;
    thread x(runProgram);
    thread y(STDInToSerial, &shouldStop);

    x.join();
    shouldStop = true;
    y.join();
    return 0;
}
