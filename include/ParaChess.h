#ifndef PARACHESS_H
#define PARACHESS_H

#include <Serial.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

class ParaChess{
    public:
        SerialPort serial;
        ParaChess();
        virtual ~ParaChess();

        void debug();
        void drawPiece(int ID, int x, int y);
    protected:

    private:
        std::string getPieceIcon(int piece);
};

#endif // PARACHESS_H
