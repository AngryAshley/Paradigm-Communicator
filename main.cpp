#include <iostream>
#include <ctime>
#include <windows.h>
#include <fstream>
#include <vector>
#include <unistd.h>

#include <Serial.h>
#include <e-mail.h>
#include <tools.h>





using namespace std;

/// Program specific variables ///
char incomingData[MAX_DATA_LENGTH];
int serialBaud = 6.666666;
string PCS_ver="0.1.0 A";
string path_exe="";

typedef vector<string> strvec;

/// User-end variables ///
int sys_comNum=4;
string misc_color="\e[1;32m\e[40m";
string msg_title="Welcome to this Paradigm Communicator server."; //Fetch, but have a default
string msg_MOTD=""; //Fetch, no default
char *port_name = "\\\\.\\COM6";
string msg_welcome="Welcome, ";

/// Session Variables ///
string uname="";
string passwd="";
string cmd_temp="";
string cmd[8];
string dir_arr[1024];
char msg_buf[255];
bool uname_good=false;
bool passwd_good=false;
vector<string> folder;

/// System initialization ///
SerialPort serial;
Tools tools;
e_mail mail;


void getPath() {
char path_exe_temp[1024];
GetModuleFileName(NULL, path_exe_temp, 1024);
int pos=string(path_exe_temp).find_last_of("\\/");
path_exe=string(path_exe_temp).substr( 0, pos+1);
}

string setting_read(string setting, string path){
    string line;
    ifstream file;
    //cout<<"Opening file "<<path<<" from "<<path_exe<<endl;
    string fullPath = path_exe + path;
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

void loadSettings(){
    getPath();
    if(setting_read("msg_title", "\\Settings\\settings.txt")!="*"){
        msg_title=setting_read("msg_title", "\\Settings\\settings.txt");
    }
    string tempPort_name = setting_read("ser_port", "\\Settings\\settings.txt");
    msg_MOTD=setting_read("msg_MOTD", "\\Settings\\settings.txt");
    if(setting_read("sys_OS", "\\Settings\\settings.txt")=="win"){
        //port_name = "\\\\.\\COM4";
        //port_name += const_cast<char*>(tempPort_name.c_str());
        //port_name = "\\\\.\\";
        //strcat(port_name, const_cast<char*>(tempPort_name));
    }
    //*port_name=const_cast<char*>(tempPort_name.c_str());
    //port_name=const_cast<char*>(tempPort_name);
    for(int i=0;i<strlen(port_name);i++){
        printf("%c", port_name[i]);
    }
};

void read_directory(const string& name, strvec& v){
    string pattern(name);
    int i = 0;
    pattern.append("\\*");
    WIN32_FIND_DATA data;
    HANDLE hFind;
    if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
        do {
            dir_arr[i] = data.cFileName;
            v.push_back(data.cFileName);
            i++;
        } while (FindNextFile(hFind, &data) != 0);
        FindClose(hFind);
    }
}

void login_finalize(){
    string path = string(path_exe)+string("Users\\")+string(uname)+string("\\email\\");
    read_directory(path,folder);
    mail.serial = serial;
    mail.mailFolder = folder;
    mail.path = path;
    printf("path: %s",path.c_str());
    mail.getNewMail();
}

int login(){
    serial.write("Username: ");
    uname=serial.readLine(0);
    serial.write("Password: ");
    passwd=serial.readLine(1);
    serial.write("\n");

    switch(query(uname, passwd)){
    case 0: serial.print(msg_welcome + uname); login_finalize(); return 1; break;
    case 1: serial.print("Wrong password"); login(); break;
    case 2: serial.print("Wrong username"); login(); break;
    default: serial.print("Unexpected Error"); login(); break;
    }
}

int CLI(){
    fill_n(cmd, 8, "");



    serial.write(uname);
    serial.write(">");

    cmd_temp=serial.readLine(0);
    cmd_temp = tools.to_upper(cmd_temp);
    tools.splitString(cmd_temp, cmd);


    if(cmd[0] == "TEST"){
        serial.print("pooped");

    } else if(cmd[0]=="EXIT") {
        serial.print("Logging off... Bye!");
        Sleep(1000);
        serial.write("+++");
        Sleep(1000);
        serial.write("ATH0\r");
        return 1;

    } else if(cmd[0]=="BEEP"){
        //serial.print("\a");
        serial.print(serial.getKey());

    } else if(cmd[0]=="EMAIL"){
        if(cmd[1]=="CHECK"){
            mail.getNewMail();

        } else if(cmd[1]=="HELP"){
            mail.help();
        } else if(cmd[1]=="INBOX"){
            mail.inbox();
        } else {
            serial.write("Bad parameter - ");
            serial.write(cmd[1]);
            serial.print("");
        }


    } else {
        serial.write("Bad command - ");
        serial.write(cmd[0]);
        serial.print("");
    };

    serial.print("");
    CLI();
}


int main()
{
    loadSettings();
    cout<<"Establishing connection with "<<&port_name<<"... ";
    serial.connect(port_name);
    cout<<"OK"<<endl;

    serial.print(misc_color);
    serial.write("\e[2J");
    serial.print("Paradigm Communicator " + PCS_ver);
    serial.print("The date is " + tools.date());
    serial.print("");
    serial.print(msg_title);
    serial.print("");

    if(login() == 1){
        CLI();
    }
    return 0;
}
