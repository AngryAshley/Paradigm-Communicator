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
        std::future<int> fut = std::async(this->waitForKey);
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



return;
}



void ASHLEY::printBorders(){
           ///11111111111111111111111111111111111111112222222222222222222222222222222222222222
serial.print("         __               __                                   ");
serial.print("    /|  /     |  |  |    |    \\  /                             ");
serial.print("   / |  \\__   |__|  |    |__   \\/                              ");
serial.print("  /__|     \\  |  |  |    |     /                              ");
serial.print(" /   |   __/  |  |  |__  |__   /    C o n t r o l   p a n e l ");
serial.print("");
serial.print("嬪 Status 様様様様様様様様様様邑");
serial.print("�                              �");
serial.print("�   CPU:                       �");
serial.print("�                              �");
serial.print("�                              �");
serial.print("�                              �");
serial.print("青陳陳陳陳陳陳陳陳陳陳陳陳陳陳潰");


}
