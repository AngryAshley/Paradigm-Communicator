#include "Serial.h"

SerialPort::SerialPort(){
    this->connected = false;
}

SerialPort::connect(char *portName){
    this->connected = false;

    this->handler = CreateFileA(static_cast<LPCSTR>(portName),
                                GENERIC_READ | GENERIC_WRITE,
                                0,
                                NULL,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);
    if (this->handler == INVALID_HANDLE_VALUE){
        if (GetLastError() == ERROR_FILE_NOT_FOUND){
            printf("SERIAL_ERROR: Handle was not attached, %s not available\n", portName);
        } else {
            printf("SERIAL_ERROR: %s",strerror(GetLastError()));
        }
    } else {
        DCB dcbSerialParameters = {0};
        if (!GetCommState(this->handler, &dcbSerialParameters)) {
            printf("failed to get current serial parameters");
        } else {
            dcbSerialParameters.BaudRate = CBR_300;
            dcbSerialParameters.ByteSize = 8;
            dcbSerialParameters.StopBits = ONESTOPBIT;
            dcbSerialParameters.Parity = NOPARITY;
            dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

            if (!SetCommState(handler, &dcbSerialParameters))
            {
                printf("ALERT: could not set Serial port parameters\n");
            } else {
                this->connected = true;
                PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
                Sleep(ARDUINO_WAIT_TIME);
            }
        }
    }
}

SerialPort::~SerialPort(){
    if (this->connected){
        this->connected = false;
        CloseHandle(this->handler);
    }
}

int SerialPort::readSerialPort(char *buffer, unsigned int buf_size){
    DWORD bytesRead;
    unsigned int toRead;

    ClearCommError(this->handler, &this->errors, &this->status);

    if (this->status.cbInQue > 0){
        if (this->status.cbInQue > buf_size){
            toRead = buf_size;
        } else toRead = this->status.cbInQue;
    }
    if (ReadFile(this->handler, buffer, toRead, &bytesRead, NULL)) return bytesRead;
    return 0;
}

bool SerialPort::writeSerialPort(char *buffer, unsigned int buf_size){
    DWORD bytesSend;

    if (!WriteFile(this->handler, (void*) buffer, buf_size, &bytesSend, 0)){
        ClearCommError(this->handler, &this->errors, &this->status);
        return false;
    } else return true;
}

bool SerialPort::isConnected(){
    return this->connected;
}


/// Extra Functions ///

void SerialPort::write(std::string input){
    const char *text = input.c_str();
    int len = input.std::string::size();
    for(int i=0;  i<len; i++){
        printf("%c",text[i]);

    };
    char * convert;
    convert = const_cast<char *>(text);
    this->writeSerialPort(convert,len);
}

void SerialPort::print(std::string input){
    write(input);
    printf("\n");
    this->writeSerialPort("\r\n",2);
}

std::string SerialPort::readLine(int option){
    char* buf;
    *buf = NULL;
    bool shouldLoop = true;
    std::string out;
    while(shouldLoop){
        bool shouldPrint = true;
        this->readSerialPort(buf,1);
        switch(*buf){
            case '\r': this->writeSerialPort("\r\n",2); shouldLoop=false; break;
            case '\n': this->writeSerialPort("\r\n",2); shouldLoop=false; break;
            case NULL: shouldPrint=false; break;
            case '\x08': out = out.std::string::substr(0, out.std::string::size()-1);
                         if(option==2)break;
                         this->writeSerialPort("\x08 \x08",3);
                         *buf = NULL;
                         shouldPrint=false;
                         break;
        }
        if(*buf!=NULL)printf("%02X", *buf);
        if(shouldPrint && shouldLoop){
            out += *buf;
            switch(option){
            case 0: this->writeSerialPort(buf,1); break;
            case 1: this->writeSerialPort("*",1); break;
            case 2: this->writeSerialPort(" \x08",2); break;
            }
            *buf=NULL;
        };
    }
    return out;
}

std::string SerialPort::getKey(){
    char* buf;
    //std::string output ="";
    int i=0;
    buf = (char*) malloc(4);
    char* out = NULL;
    *buf = NULL;
    while(*buf==NULL){
        i=this->readSerialPort(buf,1);
    }
        std::string output(1, *buf);
        //output += out;
    //printf(" buf: %02x ", *buf);
    if(*buf=='\x1b'){
        output="";
        output += *buf;
        *buf = NULL;
        while(*buf==NULL){
            i=this->readSerialPort(buf,1);
        }
        output += *buf;
        *buf = NULL;
        while(*buf==NULL){
            i=this->readSerialPort(buf,1);
        }
        output += *buf;
    }
    /*
    for(int s=0; s<3;s++){
        printf("%02x", output[s]);
    }
    printf(" | ");
    printf("%s",output);
    */

    return output;
}
