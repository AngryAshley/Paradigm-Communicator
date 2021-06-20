#include "ParaChess.h"

ParaChess::ParaChess(){
}
ParaChess::~ParaChess(){
}


std::string ParaChess::getPieceIcon(int piece){
    std::string icon;
    switch(piece){
        case 1: icon="    \xEA \xDC\xDB\xDC"; break; //Pawn
        case 2: icon="\xB7\xB7\xB7\xDE\xDB\xDD\xDB\xDB\xDB"; break;//Rook
        case 3: icon="\xDC\xDB\xDC \xDB\xDF\xDB\xDB\xDC"; break;//Knight
        case 4: icon="(^) \xDB \xDC\xDB\xDC"; break;//Bishop
        case 5: icon="\xEC\xA7\xEC \xDB \xDC\xDB\xDC"; break; //Queen
        case 6: icon=" \xC5 \xDF\xDB\xDF\xDC\xDB\xDC"; break;//King
    }
    return icon;
}

void ParaChess::drawPiece(int ID, int x, int y){
    int pos=0;
    for(int i=0; i<3;i++){
        serial.write("\e["+std::to_string(x+i)+";"+std::to_string(y)+"H");
        for(int j=0;j<3;j++){
                std::string strtemp;
                strtemp = getPieceIcon(ID)[pos];
            serial.write(strtemp);
            pos++;
        }
    }
}

void ParaChess::debug(){
    drawPiece(1,3,3);
    drawPiece(2,6,3);
    drawPiece(3,9,3);
    drawPiece(4,12,3);
    drawPiece(5,15,3);
    drawPiece(6,18,3);

}
