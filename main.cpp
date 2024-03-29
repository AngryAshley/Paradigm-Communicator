#include <iostream>
#include <ctime>
#include <windows.h>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <cstdlib>

#include <Serial.h>
#include <e-mail.h>
#include <tools.h>
#include <fileSystem.h>
#include <users.h>
#include <ASHLEY.h>
#include <SAS.h>
#include "ParaChess.h"




using namespace std;

/// Program specific variables ///
char incomingData[MAX_DATA_LENGTH];
int serialBaud = 6.666666;
string PCS_ver="1.0.0 A";
string path_exe="";
bool ASHLEYmode=true;

/// Debugging variables ///
bool autoLogin=false;
string autoLoginName = "meep";
string autoLoginPass = "poop";

typedef vector<string> strvec;

/// System initialization ///
SerialPort serial;
serialTools stools;
Tools tools;
e_mail mail;
fileSystem fs;
Users users;
ASHLEY ashley;
SAS Sas;
ParaChess pchess;

/// User-end variables ///
int sys_comNum=4;
string misc_color="\e[1;32m\e[40m";
string misc_defColor="\e[1;37m\e[44m";
string msg_title="Welcome to this Paradigm Communicator server."; //Fetch, but have a default
bool msg_title_bigLogo=true;
string msg_MOTD="\e[2J\e[0;0HThe date is " + tools.date(); //Fetch, no default
string port_name="COM4";
string msg_welcome="Welcome, ";

/// Session Variables ///
string uname="";
string passwd="";
string cmd_temp="";
string cmd[8];
string dir_arr[1024];
string homefolder="";
string temppath;
char msg_buf[255];
bool uname_good=false;
bool passwd_good=false;
vector<string> folder;



void getPath() {
char path_exe_temp[1024];
GetModuleFileName(NULL, path_exe_temp, 1024);
int pos=string(path_exe_temp).find_last_of("\\/");
path_exe=string(path_exe_temp).substr( 0, pos+1);
tools.pathexe=path_exe_temp;
}

void loadSettings(){
    printf("Loading settings\n");

    getPath();
    tools.pathexe = path_exe;
    printf("[INFO] Path is %s\n",path_exe.c_str());
    if(tools.setting_read("msg_title", "\\Settings\\title.txt")!="*"){
        msg_title=tools.setting_read("msg_title", "\\Settings\\title.txt");
    }
    string tempPort_name = tools.setting_read("ser_port", "\\Settings\\settings.txt");
    //msg_MOTD=setting_read("msg_MOTD", "\\Settings\\settings.txt");
    printf("OS=%s\n",tools.setting_read("sys_os", "\\Settings\\settings.txt").c_str());
    if(tools.setting_read("sys_os", "\\Settings\\settings.txt")=="win"){
        port_name="COM"+tools.setting_read("ser_port", "\\Settings\\settings.txt");
        printf("Port is %s\n",port_name.c_str());
    }
    ASHLEYmode=atoi(tools.setting_read("AshleyMode", "\\Settings\\settings.txt").c_str());
    printf("Done loading settings\n");
};



//================================================================================================== CLI block ================================\\\

int CLI(){
    fill_n(cmd, 8, "");

    if(atoi(tools.setting_read("archival", "\\Users\\"+uname+"\\settings.txt").c_str())>0){if(Sas.check(false)>0)Sas.check();}
    if(mail.getNewMail(false)>0){mail.getNewMail();}

    temppath=fs.cd;
    if(temppath.size()>=homefolder.size()){
        temppath.erase(0, homefolder.size());
    }
    serial.write(uname+"@"+temppath+">");

    cmd_temp=serial.readLine(0);
    tools.splitString(cmd_temp, cmd," ");
    cmd[0]=tools.to_upper(cmd[0]);

    if(cmd[0] == "VER"){
        serial.print("Paradigm communicator "+PCS_ver);
    } else if(cmd[0]=="EXIT") {
        stools.closeConn("Logging off... Bye!");
        return 0;

    } else if(cmd[0]=="BEEP"){
        serial.print("\a");
        //serial.print(serial.getKey());

    } else if(cmd[0]=="EMAIL"){
        cmd[1]=tools.to_upper(cmd[1]);
        if(cmd[1]=="CHECK"){
            mail.getNewMail();

        } else if(cmd[1]=="HELP"){
            mail.help();
        } else if(cmd[1]=="INBOX"||cmd[1]==""){
            mail.inbox();
        } else {
            stools.throwError(0,cmd[1]);
        }

    } else if(cmd[0]=="LOGOFF"){
        serial.print("Logging off, bye!");
        uname = "";
        passwd = "";
        homefolder = "";
        fs.cd = "";
        temppath = "";
        mail.reset();
        folder.clear();
        Sleep(1000);
        printf("Safe Log-off was succesful!\n");
        return 1;

    } else if(cmd[0]=="DIR"){
        fs.showDir();

    } else if(cmd[0]=="CD"){
        printf("%s",cmd[1].c_str());
        if(cmd[1]==".."){
            if(fs.cd == homefolder && users.auth(uname)>=9){
                stools.throwError(11);
            } else {
                fs.changeDir(cmd[1]);
            }
        } else {
            //serial.print(cmd[1]);
            fs.changeDir(cmd[1]);
        }

    } else if(cmd[0]=="PRINT"){
        if(fs.fileExists(fs.cd+cmd[1])){
            fs.printBigFile(fs.cd+cmd[1]);
        } else {
            stools.throwError(20,cmd[1]);
        }
    } else if(cmd[0]=="HELP"){
        fs.printBigFile(string(path_exe)+string("\\Settings\\help.txt"));

    } else if(cmd[0]=="RUN"){

        /// implementeer iets leuks met spawn, gebruik async voor full-duplex pipeline wat erg belangrijk is


    } else if(cmd[0]=="USER"){
        cmd[1]=tools.to_upper(cmd[1]);
        if(cmd[1]=="CREATE"){
            if(users.auth(uname)<3){
                users.makeUser();
            } else {
                stools.throwError(10);
            }
        } else if(cmd[1]=="PURGE"){
            if(users.auth(uname)<3){
                users.purgeUser();
            } else {
                stools.throwError(10);
            }
        } else {
            stools.throwError(0,cmd[1]);
        }
    } else if(cmd[0]=="SAS"||cmd[0]=="ARCHIVE"){
        cmd[1]=tools.to_upper(cmd[1]);
        if(cmd[1]==""){
            Sas.menu(users.auth(uname),atoi(tools.setting_read("archival", "\\Users\\"+uname+"\\settings.txt").c_str()),uname);
        } else if(cmd[1]=="INBOX"){
            if(atoi(tools.setting_read("archival", "\\Users\\"+uname+"\\settings.txt").c_str())>0){
                printf("calling inbox");
                Sas.inbox();
            } else {
                stools.throwError(12,"only archive personnel is allowed to access the inbox.");
            }
        } else if(cmd[1]=="CHECK"){
            if(atoi(tools.setting_read("archival", "\\Users\\"+uname+"\\settings.txt").c_str())>0){
                Sas.check();
            } else {
                stools.throwError(12,"only archive personnel is allowed to access the inbox.");
            }
        }

    } else if(cmd[0]=="EDIT"){
        fs.fileEditor(cmd[1]);
    } else if(cmd[0]=="HWOOD"){
        cmd[1]=tools.to_upper(cmd[1]);
        if(       cmd[1]=="ASHLEY"){
            ashley.controlPanel();
        } else if(cmd[1]=="TAPE"){
            ashley.tapeLeak();
            } else if(cmd[1]=="RADAR"){
            ashley.printCanvas(2);
            serial.getKey();
        } else if(cmd[1]=="MELTDOWN"){
            string command = string(path_exe)+string("\\Resources\\meltdown.mp3");
            getPath();
            fs.printBigFile(string(path_exe)+string("Resources\\meltdown.txt"));
            system(command.c_str());
            serial.getKey();
            serial.write("\e[0m"+misc_defColor+"\e[2J");
        } else {
            stools.throwError(255,"No hollywood scene found for "+cmd[1]);
        }
    } else if(cmd[0]=="DB"){
        if(cmd[1]!=""){

            fs.db(cmd[1],atoi(tools.setting_read("authLevel", "\\Users\\"+uname+"\\settings.txt").c_str()));
        } else {
            serial.print("Please specify database");
        }
    } else if(cmd[0]=="CHESS"){
        pchess.debug();
    } else {
        stools.throwError(1,cmd[0]);
    };


    serial.print("");
    int ret = CLI();
    if(ret<2) return ret;
    return 255; //something went horribly wrong
}

//================================================================================================== Login block ================================\\\

void login_finalize(){
    homefolder = string(path_exe)+string("Users\\")+string(uname)+string("\\");
    folder.clear();
    string path = homefolder+string("email\\");
    fs.read_dir_vect(path,folder);
    mail.mailFolder = folder;
    mail.path = path;
    mail.defColor = misc_defColor;
    fs.defColor = misc_defColor;
    fs.cd = homefolder;

    if(mail.getNewMail(false)<0){mail.getNewMail();}
    mail.getNewMail();

}

int login(){
    int ret=255;
    serial.write("Username: ");
    uname=serial.readLine(0);
    if(uname=="exit"){
        stools.closeConn("Closing connection...");
        return 0;
    }
    serial.write("Password: ");
    passwd=serial.readLine(1);
    serial.write("\n");

    switch(users.query(uname, passwd)){
    case 0: return 1; break;
    case 1: serial.print("Wrong password"); ret = login(); break;
    case 2: serial.print("Wrong username"); ret = login(); break;
    default: serial.print("Unexpected Error"); ret = login(); break;
    }
    return ret;
}


void loginPrompt(){
    if(!autoLogin){
    serial.write("\e[0m"+misc_color+"\e[2J\e[0;0H");
    serial.print("Paradigm Communicator " + PCS_ver);
    serial.print("The date is "+tools.date()+"\r\n");
    if(msg_title_bigLogo){
        mail.inbox_drawContent(string(path_exe)+string("\\Settings\\title.txt"),0);
    } else {
        serial.print(msg_title);
    }
    serial.print("");

    if(login() == 1){
            if(msg_MOTD!="") serial.print(msg_MOTD);
            serial.print(msg_welcome + uname);
            login_finalize();
            int ret = CLI();
        if(ret==1){
            loginPrompt();
        } else {
            printf(" error %i ",ret);
        }
    }
    } else {
        serial.print(misc_defColor+"\e[2JAuto-logged in>\n");
        uname=autoLoginName;
        passwd=autoLoginPass;
        CLI();
    }
}


int main(){
    printf("Paradigm communicator %s\n",PCS_ver.c_str());

    loadSettings();
    printf("Establishing connection with %s... ",port_name.c_str());
    serial.connect(port_name);
    printf("OK\n");
        stools.serial = serial;
        mail.serial = serial;
        mail.fs = fs;
        fs.serial = serial;
        users.serial = serial;
        users.tools = tools;
        users.fs = fs;
        ashley.serial = serial;
        ashley.tools = tools;
        Sas.fs = fs;
        Sas.tools = tools;
        Sas.serial = serial;
        Sas.sTools = stools;
        Sas.path_exe=path_exe;
        pchess.serial = serial;

    if(ASHLEYmode){
        if(ashley.ashleyBoot()!=0){
            return 0;
        }
    } else {
        printf("ASH disabled\n");
    }
    loginPrompt();

    printf("User disconnected");
    return 0;
}
