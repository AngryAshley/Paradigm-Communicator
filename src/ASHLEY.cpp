#include "ASHLEY.h"

ASHLEY::ASHLEY()
{
    //ctor
}

ASHLEY::~ASHLEY()
{
    //dtor
}

int ASHLEY::ashleyBoot(){
    int timer =(rand() % 10) + 1; ///Random Generator
    timer = 11; ///Uncomment if timeout


    serial.write("\e[0m\e[1;32m\e[40m\e[2J\e[0;0H");
    serial.getKey();
    serial.write("\e[12B                        Accessing networking subsystem");
    Sleep(1537);
    serial.write("\e[80D                           Establishing connection      ");
    for(int i=0; i<=10; i++){
        if(i%4==0){
            serial.write("\e[6D      \e[6D");
        } else {
            serial.write(" .");
        }
        Sleep(1000);
        if(i==timer){
            return 0;
        };
    }
    serial.write("\a\r\n              Error establishing connection - connection timed out");
    serial.getKey();
    serial.print("\r\n\r\n  - MANUAL CONTROL MODE ENABLED - ");
    while(true){
        serial.write("@>");
        std::string cmd_temp=serial.readLine(0);
        if(cmd_temp=="disconnect"){
            for(int i=0; i<=25; i++){
                serial.print("");
            }
            serial.write("\e[2J");
            return 1;
        }
        Sleep(10);
        cmd_temp="";
    }
}


std::string ASHLEY::waitForKey(){
    std::string ret = serial.getKey();
    running=false;
    serial.write("BOOP");
    if(ret=="e"){
        terminate=true;
    }
    return ret;
}
bool ASHLEY::waitForMaster(){
    system("pause");
    return true;
}

void ASHLEY::tapeLeakLog(){
    std::string date="9/26/1993 11:0";
    if(second==10){date="9/26/1993 11:";};
    serial.getKey();
    serial.write(date+std::to_string(second)+": ");
    std::string lin = serial.readLine(0);
    log.push_back(date+std::to_string(second)+": "+lin);
    serial.write("\e[38C");
    second++;
}

void ASHLEY::tapeLeak(){
    std::string mpcPath= "\"D:\\Program Files\\MPC-HC\\MPC-HC64\" ";
    std::string arguments=" /play /close";
    std::string command = mpcPath+std::string(tools.pathexe)+std::string("\\Resources\\tape\\SD1.wav")+arguments;

    serial.write("\e[0m\e[1;37m\e[44m\e[2J");
    this->printCanvas(1);
    this->tapeLeakLog();
    this->tapeLeakLog();
    this->tapeLeakLog();
    this->tapeLeakLog();///preparation, four lines.
    this->waitForMaster();
    system(command.c_str());     ///Sierra acknowledge
    this->tapeLeakLog();
    this->waitForMaster();
    command = mpcPath+std::string(tools.pathexe)+std::string("\\Resources\\tape\\NF1.wav"+arguments); ///November reached spot
    system(command.c_str());
    this->waitForMaster();
    command = mpcPath+std::string(tools.pathexe)+std::string("\\Resources\\tape\\NF2.wav"+arguments); ///November acknowledge
    system(command.c_str());
    this->tapeLeakLog();
    serial.getKey();
    this->printCanvas(2);
    this->waitForMaster();
    command = mpcPath+std::string(tools.pathexe)+std::string("\\Resources\\tape\\SD2.wav"+arguments); ///Sierra tracking info
    system(command.c_str());
    serial.getKey();
    this->printCanvas(1);
    this->tapeLeakLog();
    this->waitForMaster();
    command = mpcPath+std::string(tools.pathexe)+std::string("\\Resources\\tape\\SD3.wav"+arguments); ///Sierra ask for acknowledge
    system(command.c_str());
    this->waitForMaster();
    command = mpcPath+std::string(tools.pathexe)+std::string("\\Resources\\tape\\SD4.wav"+arguments); ///Sierra got visuals
    system(command.c_str());
    this->tapeLeakLog();
    this->waitForMaster();
    command = mpcPath+std::string(tools.pathexe)+std::string("\\Resources\\tape\\NF3.wav"+arguments); ///November acknowledge visuals
    system(command.c_str());
    serial.getKey();
    this->printCanvas(2);
    serial.getKey();
    this->printCanvas(1);
    this->waitForMaster();
    command = mpcPath+std::string(tools.pathexe)+std::string("\\Resources\\tape\\SD5.wav"+arguments); ///Sierra requesting strike
    system(command.c_str());
    this->tapeLeakLog();
    std::cin.ignore();
    this->printCanvas(3);
    serial.readLine(1);
    this->printCanvas(1);
    this->waitForMaster();
    command = mpcPath+std::string(tools.pathexe)+std::string("\\Resources\\tape\\SD6.wav"+arguments); ///Sierra strike auth copy
    system(command.c_str());
    this->waitForMaster();
    command = mpcPath+std::string(tools.pathexe)+std::string("\\Resources\\tape\\SD7.wav"+arguments); ///Sierra tango down
    system(command.c_str());
    this->waitForMaster();
    command = mpcPath+std::string(tools.pathexe)+std::string("\\Resources\\tape\\NF4.wav"+arguments); ///November acknowledge
    system(command.c_str());
    this->tapeLeakLog();
    this->waitForMaster();
    command = mpcPath+std::string(tools.pathexe)+std::string("\\Resources\\tape\\NF5.wav"+arguments); ///November captured
    system(command.c_str());
    this->tapeLeakLog();
    serial.getKey();
    serial.write("\e[2J\e[1;1H");
}

void ASHLEY::controlPanel(){
    serial.write("\e[0m\e[1;37m\e[44m\e[2J");
    this->printCanvas(0);
    std::string output = "";
    std::thread (waitForKey, this).detach();
    while(true){
        if(running){
            serial.write(".");
            Sleep(1000);
        }


        if(!running && terminate){
            break;
        } else if(!running && !terminate){





            Sleep(1000);
            std::thread (waitForKey, this).detach();
            running=true;
        }
    }

return;
}



void ASHLEY::printCanvas(int i){
    switch(i){
        case 0:    ///11111111111111111111111111111111111111112222222222222222222222222222222222222222
        serial.print("         __               __                                   ");
        serial.print("    /|  /     |  |  |    |    \\  /                             ");
        serial.print("   / |  \\__   |__|  |    |__   \\/                              ");
        serial.print("  /__|     \\  |  |  |    |     /                              ");
        serial.print(" /   |   __/  |  |  |__  |__  /    C o n t r o l   p a n e l ");
        serial.print("");
        serial.print("�� Status ��������������������Ϳ");
        serial.print("�                              �");
        serial.print("�   CPU:                       �");
        serial.print("�                              �");
        serial.print("�                              �");
        serial.print("�                              �");
        serial.print("��������������������������������");
        break;
        case 1:    ///11111111111111111111111111111111111111112222222222222222222222222222222222222222
        serial.print("\e[44m\e[1;37m\e[2J\e[1;1H�� SCS Control panel ��������������������������������������������������������Ϳ");
        serial.print("�\e[2;80H�");
        serial.print("�� Agents ��������������������������������������������������������������������Ĵ");
        serial.print("� Name   Status  Position � Name   Status  Position � Name   Status  Position  �");
        serial.print("�\e[25C�\e[25C�\e[25C �");
        serial.print("�\e[25C�\e[25C�\e[25C �");
        serial.print("�\e[25C�\e[25C�\e[25C �");
        serial.print("������������������������������������������������������������������������������Ĵ");
        serial.print("�\e[9;80H�");
        serial.print("� �� Target ���������������������Ϳ �� Log ���������������������������������Ϳ �");
        serial.print("� �\e[11;35H� �\e[11;78H� �");
        serial.print("� �\e[12;35H� �\e[12;78H� �");
        serial.print("� �\e[13;35H� �\e[13;78H� �");
        serial.print("� �\e[14;35H� �\e[14;78H� �");
        serial.print("� �\e[15;35H� �\e[15;78H� �");
        serial.print("� �\e[16;35H� �\e[16;78H� �");
        serial.print("� �\e[17;35H� �\e[17;78H� �");
        serial.print("� �\e[18;35H� �\e[18;78H� �");
        serial.print("� �\e[19;35H� �\e[19;78H� �");
        serial.print("� �\e[20;35H� �\e[20;78H� �");
        serial.print("� �\e[21;35H� �\e[21;78H� �");
        serial.print("� �\e[22;35H� �\e[22;78H� �");
        serial.print("� ��������������������������������� ������������������������������������������ �");
        serial.write("��������������������������������������������������������������������������������\e[5;3H");
        serial.write("SD277  OK      H6\e[5;29HNB165  OK      G5\e[5;55HNR175  OK      G5\e[6;3H");
        serial.write("SE683  OK      H6\e[6;29HNE568  OK      G5\e[6;55HNK294  OK      G5\e[7;3H");
        serial.write("NF538  OK      H6\e[7;29HNU724  OK      G5\e[7;55HNJ771  OK      G5\e[11;5H");
        serial.write("Specimen ID: 59294142\e[12;5HSpecies:     Shapeshifter\e[13;5HStatus:      OK\e[14;5HTracking:    LOCKED\e[15;5H - Conf.:    89%\e[16;5H");
        serial.write(" - Pos.:     H7");
        serial.write("\e[11;39H");
        for(int i=0; i<log.size(); i++){
            serial.print(log[i].c_str());
            serial.write("\e[38C");
        }
        break;
        case 2:    ///11111111111111111111111111111111111111112222222222222222222222222222222222222222
        serial.write("\e[40m\e[1;32m\e[2J;\e[1;1H");
        serial.print(" � RADAR ����������������������������������������������������������������������");
        serial.print(" �\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�");
        serial.print(" �\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�");
        serial.print("�������������������������������������������������������������������������������");
        serial.print(" �\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�");
        serial.print(" �\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�");
        serial.print(" �\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�");
        serial.print("�������������������������������������������������������������������������������");
        serial.print(" �\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�");
        serial.print(" �\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�");
        serial.print(" �\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�");
        serial.print("�������������������������������������������������������������������������������");
        serial.print(" �\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�");
        serial.print(" �\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�");
        serial.print(" �\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�");
        serial.print("�������������������������������������������������������������������������������");
        serial.print(" �\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�");
        serial.print(" �\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�");
        serial.print(" �\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�");
        serial.print("�������������������������������������������������������������������������������");
        serial.print(" �\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�");
        serial.print(" �\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�");
        serial.print(" �\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�\e[6C�");
        serial.write("\e[0m\e[42m\e[30m [ SETTINGS ] [ ZOOM ] [ POSITION ] [ UP/DOWNLINK ]                           \e[40m\e[1;32m");
        serial.write("\e[2;5HA\e[6CB\e[6CC\e[6CD\e[6CE\e[6CF\e[6CG\e[6CH\e[6CI\e[6CJ\e[6CK");
        serial.write("\e[6;1H2\e[10;1H3\e[14;1H4\e[18;1H5\e[22;1H6");
        serial.write("\e[22;67H0");
        serial.write("\e[24;1H \e[5m[ SETTINGS ] \e[0m");
        break;
        case 3:    ///11111111111111111111111111111111111111112222222222222222222222222222222222222222
        serial.write("\a\e[41m\e[10;20H�� NOTICE ����������������������������ͻ");
        serial.write("\e[11;20H� Orbital strike request by SD277      �");
        serial.write("\e[12;20H� Do you want to authorize the request?�");
        serial.write("\e[13;20H� [__________________________________] �");
        serial.write("\e[14;20H�                       [ NO ] [>YES<] �");
        serial.write("\e[15;20H��������������������������������������ͼ\e[13;24H");
        break;
    }
}
