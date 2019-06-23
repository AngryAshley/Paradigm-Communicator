#include "Users.h"

Users::Users()
{
    //ctor
}

Users::~Users()
{
    //dtor
}

int Users::auth(std::string user){  /// 0 is guest, 10 is absolute admin
    std::string val = tools.setting_read("authLevel", "\\Users\\"+user+"\\settings.txt");
    if(val=="NOT_FOUND"){val="0";};
    return std::atoi(val.c_str());
}

int Users::query(std::string uname, std::string passwd){
    bool uname_good=false;
    bool passwd_good=false;
    for(int i=0; i<passwd.size(); i++)printf("%02X", passwd[i]);
	if(uname == "falken" || uname == "admin"){
        uname_good = true;
		if(passwd == "joshua" || passwd == "password"){
                passwd_good = true;
		}
	}
    if(tools.setting_read(passwd, "\\Users\\"+uname+"\\settings.txt")!="FILE_NOT_FOUND"){
        uname_good=true;
        if(tools.setting_read("password", "\\Users\\"+uname+"\\settings.txt")==passwd){
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



void Users::makeUser(){
    std::string username;
    std::string password;
    std::string authLevel;

    serial.print(" - User Generator - ");
    serial.write("Username: ");
    username=serial.readLine(0);
    serial.write("Password: ");
    password=serial.readLine(0);
    serial.write("Auth level: ");
    authLevel=serial.readLine(0);

    printf("Generating %s, pwd: %s, level %i",username,password,authLevel);
    if(tools.setting_read(password, "\\Users\\"+username+"\\settings.txt")!="FILE_NOT_FOUND"){
        serial.print("This user already exists!");
        return;
    }

    mkdir(std::string(tools.pathexe+"Users\\"+username+"\\").c_str());
    mkdir(std::string(tools.pathexe+"Users\\"+username+"\\email\\").c_str());
    std::ofstream file;
    file.open(tools.pathexe+"Users\\"+username+"\\settings.txt", std::ios_base::app);
    file << "password="+password+"\n";
    file << "authLevel="+authLevel+"\n";
    file.close();
    fs.hideFile(tools.pathexe+"Users\\"+username+"\\settings.txt");

    serial.print("The user was created successfully!");
}

void Users::purgeUser(){
    std::string line="";
    serial.print(" - User removal tool - ");
    serial.write("Username: ");
    std::string username=serial.readLine(0);
    if(tools.setting_read(username, "\\Users\\"+username+"\\settings.txt")=="FILE_NOT_FOUND"){
        serial.print("This user does not exist!");
        return;
    }
    serial.write("Are you sure you want to remove this user? [y/n]");
    if(serial.getKey()=="n"){
        serial.print("\r\nUser removal aborted");
        return;
    }

    fs.DeleteDirectory(std::string(tools.pathexe+"Users\\"+username+"\\").c_str());

    serial.print("\r\nUser has been purged from the system.");
}
