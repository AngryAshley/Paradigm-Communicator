#ifndef SERIAL_H
#define SERIAL_H

#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 255

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string.h>

class SerialPort{
private:
    HANDLE handler;
    bool connected;
    COMSTAT status;
    DWORD errors;
public:
    SerialPort();
    ~SerialPort();
    int connect(std::string portName);

    int readSerialPort(char *buffer, unsigned int buf_size);
    bool writeSerialPort(char *buffer, unsigned int buf_size);
    bool isConnected();

    void print(std::string input);
    void write(std::string input);
    std::string readLine(int option, int specialChar=0);
    std::string getKey();
};

#endif // SERIALPORT_H
