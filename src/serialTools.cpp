#include "serialTools.h"

serialTools::serialTools()
{
    //ctor
}

serialTools::~serialTools()
{
    //dtor
}

int serialTools::choice(std::string prompt, std::string choices){
    char key;
    const char* ch = choices.c_str();
    serial.write(prompt+"[");
    for(int i=0;i<choices.std::string::size();i++){
        serial.write(choices.std::string::substr(0+i,1+i));
        serial.write("/");
    }
    serial.write("\x08]");
    while(choices.std::string::find(key)==std::string::npos){
        key = serial.getKey().c_str()[0];
    }
    return choices.std::string::find(key);
}

void serialTools::closeConn(std::string msg){
        if(msg=="NONE"){serial.print("Disconnecting...");}else{serial.print(msg);}
        Sleep(1000);
        serial.write("+++");
        Sleep(1010);                         ///wait 1 second at very least
        serial.write("ATH0\r");
        return;
}

std::string serialTools::getError(int type){
    switch(type){
        case   0: return "Bad parameter - #\r\n";
        case   1: return "Bad command - #\r\n";

        case  10: return "Permission denied\r\n";
        case  11: return "Access denied\r\n";
        case  12: return "Access denied, #\r\n";
        case  13: return "Access denied, you are not authorized\r\n";

        case  20: return "File # not found\r\n";
        default: return "General error #\r\n";
    }
    return "ERROR TYPE "+type;
}

void serialTools::throwError(int type, std::string info){
    if(type==255){
        serial.write(info); ///255 Custom error
    } else {
        std::string temp = this->getError(type);
        if(temp.find('#')!=0){
            serial.write(temp.substr(0,temp.find('#')));
            serial.write(info);
            serial.write(temp.substr(temp.find('#')+1));
        } else {
            serial.write(temp);
        }
    }
    //serial.print("\n\r");
    return;
}
