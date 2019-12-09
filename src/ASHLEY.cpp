#include "ASHLEY.h"

ASHLEY::ASHLEY()
{
    //ctor
}

ASHLEY::~ASHLEY()
{
    //dtor
}

std::string ASHLEY::waitForKey(){
    return serial.getKey();
}

void ASHLEY::controlPanel(){
    serial.write("\e[0m\e[1;37m\e[44m\e[2J");
    this->printBorders();
    std::string output = "";

    //HANDLE handle;
    //handle = _beginthread(thread, 0, 0);
    //handle = CreateThread(NULL, 0, thread, 0, 0, 0);

    /*
    std::future<std::string> fut=std::async(std::launch::async, &ASHLEY::waitForKey, this);
    while(running){


        std::chrono::milliseconds span (1000);
        while (fut.wait_for(span)==std::future_status::timeout){
        serial.write(".");
        }
        output = fut.get();

        if(output=="e"){
            return;
        }


    }
    */
    //std::thread key(waitForKey());
    while(running){






    }
return;
}



void ASHLEY::printBorders(){
           ///11111111111111111111111111111111111111112222222222222222222222222222222222222222
serial.print("         __               __                                   ");
serial.print("    /|  /     |  |  |    |    \\  /                             ");
serial.print("   / |  \\__   |__|  |    |__   \\/                              ");
serial.print("  /__|     \\  |  |  |    |     /                              ");
serial.print(" /   |   __/  |  |  |__  |__  /    C o n t r o l   p a n e l ");
serial.print("");
serial.print("ÚÍ Status ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¿");
serial.print("³                              ³");
serial.print("³   CPU:                       ³");
serial.print("³                              ³");
serial.print("³                              ³");
serial.print("³                              ³");
serial.print("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ");


}
