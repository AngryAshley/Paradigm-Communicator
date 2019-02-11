#ifndef E_MAIL_H
#define E_MAIL_H

#include <Serial.h>
#include <vector>
#include <string.h>
#include <iostream>
#include <tools.h>
#include <FileSystemTools.h>
#include <algorithm>
#include <array>
#include <tgmath.h>

class e_mail{
    Tools tools;
    FileSystemTools fstools;

    public:
        e_mail();
        virtual ~e_mail();

        void reset();
        std::vector<std::string> mailFolder;
        std::string path;
        SerialPort serial;

        void getNewMail();
        void help();
        void inbox();


    private:

        std::string username;
        bool logged;

        int inbox_drawMail(int index, int page);
        void mail_sort();
        int inbox_drawContent(std::string path, int page);
        void inbox_clearWindow();
        void inbox_openMail(int index);
};
/*
class Box {
   public:
      int index;      // Length of a box
      double breadth;     // Breadth of a box
      double height;      // Height of a box
};
*/
#endif // E-MAIL_H
