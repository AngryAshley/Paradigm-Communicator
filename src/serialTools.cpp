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
