#include <iostream>
#include <ctime>
#include <windows.h>
#include <Serial.h>
#include <fstream>
#include <unistd.h>

using namespace std;

/// Program specific variables ///
char incomingData[MAX_DATA_LENGTH];
int serialBaud = 6.666666;
string PCS_ver="0.1.0 A";
string uname="";
string passwd="";
bool uname_good=false;
bool passwd_good=false;

/// User-end variables ///
int sys_comNum=4;
string msg_title="Welcome to this Paradigm Communicator server."; //Fetch, but have a default
string msg_MOTD=""; //Fetch, no default
char *port_name = "\\\\.\\COM4";
string msg_welcome="Welcome, ";

/// System initialization ///
SerialPort serial(port_name);



string date(){
    time_t now = time(0);
    char* dt = ctime(&now);
    return dt;
}
string getPath() {
    char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    string::size_type pos = string( buffer ).find_last_of( "\\/" );
    return string( buffer ).substr( 0, pos+1);
}

string setting_read(string setting, string path){
    string line;
    ifstream file;
    cout<<"Opening file "<<path<<" from "<<getPath()<<endl;
    string fullPath = getPath() + path;
    string fetched_setting, var;
    file.open(fullPath);
    if (file.is_open())
	{
        for(int i = 0; file.good(); i++)
        {
            getline(file, line);
            fetched_setting = line.substr(0, line.find("=", 0));
            var = line.substr(line.find("=", 0)+1,line.size());
            if(fetched_setting==setting){
                    //cout<<"Setting found! Setting: "<<setting<<" with value "<<var<<endl;
                return var;
                break;
            }
        }
        //cout<<"Setting "<<setting<<" Not Found"<<endl;
        return "NOT_FOUND";
	} else {
	    cout<<"ERROR: Couldn't open file "<<fullPath<<", "<<strerror(errno)<<endl;
        return "FILE_NOT_FOUND";
	}
    file.close();
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
int query(string uname, string passwd){
    uname_good=false;
    passwd_good=false;
    for(int i=0; i<passwd.size(); i++)printf("%02X", passwd[i]);
	if(uname == "falken" || uname == "admin"){
        uname_good = true;
		if(passwd == "joshua" || passwd == "password"){
                passwd_good = true;
                if(uname=="falken")msg_welcome = "Greetings, professor ";
		}
	}
    if(setting_read(uname, "\\Settings\\users.txt")!="NOT_FOUND"){
        uname_good=true;
        if(setting_read(uname, "\\Settings\\users.txt")==passwd){
            passwd_good=true;
        }
    }

    if(uname_good){
        if(passwd_good){
			return 0;
		} else {
			return 1;
		}
	} else {
		return 2;
	}
}
string serialLine(int option){
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
    case 0: serialPrint(msg_welcome + uname); break;
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
    serialPrint(msg_title);
    serialPrint("");
    login();
    return 0;
}
