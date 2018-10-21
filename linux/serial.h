#ifndef SERIAL_H
#define SERIAL_H
#include <string>     // string function definitions

class Serial {
private:
    int timeout;
    int USB;
public:
    Serial(const char* device);
    void writeData(std::string str, bool isInstant);
    void writeData(std::string str);
    void giveUserResponse(bool shouldReplace, char replacement, char buf, int spot);
    std::string readData(bool shouldReplace, char replacement);
};

#endif
