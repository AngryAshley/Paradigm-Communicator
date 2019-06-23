#ifndef USERS_H
#define USERS_H

#include <tools.h>
#include <Serial.h>
#include <io.h>
#include <fileSystem.h>
#include <fstream>
#include <windows.h>


class Users
{
    public:
        Users();
        virtual ~Users();

        int auth(std::string username);
        int login();
        int query(std::string uname, std::string passwd);

        void makeUser();
        void purgeUser();

        Tools tools;
        SerialPort serial;
        fileSystem fs;

    protected:

    private:

};

#endif // USERS_H
