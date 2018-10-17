#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

using namespace std;

struct termios tio;
struct termios stdio;
struct termios old_stdio;

int tty_fd;

void initConnection (char* DEVICE_NAME) {
    // partly: https://en.wikibooks.org/wiki/Serial_Programming/Serial_Linux#termios
    tcgetattr(STDOUT_FILENO,&old_stdio);

    memset(&stdio,0,sizeof(stdio));
    stdio.c_iflag=0;
    stdio.c_oflag=0;
    stdio.c_cflag=0;
    stdio.c_lflag=0;
    stdio.c_cc[VMIN]=1;
    stdio.c_cc[VTIME]=0;
    tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
    tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);       // make the reads non-blocking

    memset(&tio,0,sizeof(tio));
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
    tio.c_lflag=0;
    tio.c_cc[VMIN]=1;
    tio.c_cc[VTIME]=5;

    tty_fd=open(DEVICE_NAME, O_RDWR | O_NONBLOCK);
    cfsetospeed(&tio,B9600);            // 9600 baud
    cfsetispeed(&tio,B9600);            // 9600 baud

    tcsetattr(tty_fd,TCSANOW,&tio);
}

void closeConnection() {
    close(tty_fd);
    tcsetattr(STDOUT_FILENO,TCSANOW,&old_stdio);
}

void sendData(char* data, int bytes) {
    int spot = 0;
    do {
        write(tty_fd,&data[spot],1);
        spot++;
    } while (spot < bytes);
}

void sendData(char* data) {
    sendData(data, (unsigned)strlen(data));
}

void readData(char* buf, int amount) {
    int charRead = 0;
    char c;
    do {
        if (read(tty_fd, &c, 1) > 0) { // check if data was sent
            strcat(buf, &c); // concat to buffer
            charRead++;
        }
    } while (charRead < amount);
}

void readDataUntil(char* buf, char endCharacter, unsigned int maximum) {
    do {
        readData(buf, 1);
    } while(buf[strlen(buf)-1] != endCharacter && strlen(buf) < maximum);
}

int main () {
    initConnection((char*)"/dev/pts/2");
    char* buf = new char[255];
    sendData((char*)"\r\nUsername: ");
    readDataUntil(buf, (char)'\x0d', 50);
    if ((string)buf == "user\r") {
        buf = new char[255];
        sendData((char*)"\r\nPassword: ");
        readDataUntil(buf, (char)'\x0d', 50);
        if ((string)buf == "password\r") {
            cout << "Success" << endl;
        } else {
            cout << "Fail" << endl;
        }
    } else {
        cout << "Fail" << endl;
    }
    closeConnection();
    return 1;
}
