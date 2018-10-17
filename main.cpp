#include <iostream>
#include <ctime>
#include <windows.h>
#include <Serial.h>

using namespace std;

int serialBaud = 6.666666;

string PCS_ver="0.0.0.1";
string uname="";
string passwd="";


string title="Welcome to this Paradigm Communicator server.";

char *port_name = "\\\\.\\COM4";
char incomingData[MAX_DATA_LENGTH];
SerialPort serial(port_name);



string date(){
    time_t now = time(0);
    char* dt = ctime(&now);
    return dt;
}

void serialWrite(string input){
    const char *text = input.c_str();
    int len = input.size();
    for(int i=0;  i<len; i++){
        printf("%c",text[i]);
        Sleep(serialBaud);
    };
    char * convert;
    convert = const_cast<char *>(text);
    serial.writeSerialPort(convert,len);
}
void serialPrint(string input){
    serialWrite(input);
    printf("\n");
    serial.writeSerialPort("\r\n",2);
}
void serialSpeed(int baud){
    serialBaud = baud;
}
int query(string uname, string passwd){
    for(int i=0; i<passwd.size(); i++)printf("%02X", passwd[i]);

if(uname == "admin" || uname == "admin\r\n"){
    if(passwd == "password" || passwd == "password\r\n"){
        return 0;
    } else {
        return 1;
    }
} else {
    return 2;
}
}
string serialLine(int option){
    /*
    int c;
    string out;
    while (c != '\n') {
        c=getchar();
        ///putchar (c);
        out += c;
    } ;
    */
    char* buf;
    *buf = NULL;
    bool shouldLoop = true;
    string out;
    while(shouldLoop){
        bool shouldPrint = true;
        serial.readSerialPort(buf,1);
        switch(*buf){
            case '\r': serial.writeSerialPort("\r\n",2); shouldLoop=false; break;
            case '\n': serial.writeSerialPort("\r\n",2); shouldLoop=false; break;
            case NULL: shouldPrint=false; break;
            case '\x08': out = out.substr(0, out.size()-1);
                         if(option==2)break;
                         serial.writeSerialPort("\x08 \x08",3);
                         *buf = NULL;
                         shouldPrint=false;
                         break;
        }
        if(*buf!=NULL)printf("%02X", *buf);
        if(shouldPrint && shouldLoop){
            out += *buf;
            switch(option){
            case 0: serial.writeSerialPort(buf,1); break;
            case 1: serial.writeSerialPort("*",1); break;
            case 2: serial.writeSerialPort(" \x08",2); break;
            }
            *buf=NULL;
        };
    }
    //cout<<out;



    return out;
}

int login(){
    serialWrite("Username: ");
    uname=serialLine(0);
    serialWrite("Password: ");
    passwd=serialLine(1);
    serialWrite("\n");

    switch(query(uname, passwd)){
    case 0: serialPrint("Welcome, " + uname); break;
    case 1: serialPrint("Wrong password"); login(); break;
    case 2: serialPrint("Wrong username"); login(); break;
    default: serialPrint("Unexpected Error"); login(); break;
    }
}

int main()
{
    serialPrint("Paradigm Communicator " + PCS_ver);
    serialPrint("The date is " + date());
    serialPrint("");
    serialPrint(title);
    serialPrint("");
    login();
    return 0;
}
