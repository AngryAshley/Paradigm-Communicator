#include <unistd.h>     // UNIX standard function definitions
#include <string.h>     // string function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
#include <iostream>     // File Descriptor handling definitions

#include "serial.h"

using namespace std;

Serial::Serial() {
    timeout = 50000;
}
void Serial::initConnection() {
    // open the USB connection `file`
    USB = open( "/dev/pts/2", O_RDWR| O_NOCTTY );
    // set the baud speed
    speed_t baudSpeed = B9600;

    // define a couple new variables
    struct termios tty;
    struct termios tty_old;
    // fill memory with 0s
    memset (&tty, 0, sizeof tty);

    // error handling
    if ( tcgetattr ( USB, &tty ) != 0 ) {
       cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << endl;
    }

    // save old tty parameters
    tty_old = tty;

    // set Baud Rate
    cfsetospeed (&tty, baudSpeed);
    cfsetispeed (&tty, baudSpeed);

    // setting other port stuff
    tty.c_cflag     &=  ~PARENB;            // Make 8n1
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;

    tty.c_cflag     &=  ~CRTSCTS;           // no flow control
    tty.c_cc[VMIN]   =  1;                  // read doesn't block
    tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
    tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

    // make raw
    cfmakeraw(&tty);

    // flush port, then applies attributes
    tcflush( USB, TCIFLUSH );
    if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) {
       std::cout << "Error " << errno << " from tcsetattr" << std::endl;
    }
}
void Serial::writeData(string str, bool isInstant) {
    char cmd[str.size() + 1];
    strcpy(cmd, str.c_str());
    for (long unsigned int i = 0; i < str.size(); i++) {
        write( USB, &cmd[i], 1);
        if (!isInstant)
            usleep(timeout);
    }
}
void Serial::writeData(string str) {
    writeData(str, false);
}
void Serial::giveUserResponse(bool shouldReplace, char replacement, char buf, int spot) {
    // if there's no replacement and we should replace, do not output anything
    if (shouldReplace && !replacement) return;
    // some special cases
    switch(buf) {
        case '\x08':    // backspace
            if (spot < 1)
                break;
            writeData((string)"\x08\x20\x08", true); // write backspace, replace character with space and set cursor back one position
            break;
        case '\0':      // skip null bytes
        case '\r':      // cariage return
        case '\n':      // newline
            break;
        default:        // any ordinary byte write (possibly encoded to replacement)
            if (shouldReplace) {
                writeData(string(1, replacement), true);
            } else {
                writeData(string(1, buf), true);
            }
    };
}
string Serial::readData(bool shouldReplace, char replacement) {
        int n = 0,
        spot = 0,
        maxSize = 255;
        char buf = '\0';

        char response[maxSize];
        memset(response, '\0', sizeof response);

        do {
            n = read( USB, &buf, 1 );
            sprintf( &response[spot], "%c", buf );
            giveUserResponse(shouldReplace, replacement, buf, spot);
            if (buf == '\x08') {
                if (spot > 0) {
                    sprintf( &response[spot], "%c", '\0' );
                    spot -= n;
                    sprintf( &response[spot], "%c", '\0' );
                }
            } else {
                spot += n;
            }
        } while( !(buf == '\r' || buf == '\n' || buf == '\0') && n > 0 && spot < maxSize);
        if (buf == '\r' || buf == '\n' || buf == '\0')
            sprintf( &response[spot], "%c", '\0' );
        if (n < 0) {
            // Some error occured
            return (string)"ERROR!!!";
        }
        else if (n == 0) {
            return (string)"";
        }
        return (string)response;
    }
