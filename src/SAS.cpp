#include "SAS.h"

SAS::SAS(){}
SAS::~SAS(){}

int SAS::check(bool verbose){
    std::vector<std::string>file;
    char temp[6];
    file = fst.file_getAll(path_exe+"\\Archive\\orderList.txt");
    if(verbose){
        if(file.size()>0){sprintf(temp,"%d",file.size());}else{temp[0]='n';temp[1]='o';}
        serial.print("There are "+std::string(temp)+" new requests for archived files.");
    }
    return file.size();
}

void SAS::inbox(){ /// only prints 22 first orders in the file
    std::vector<std::string>file;
    std::vector<std::string>fileData;
    file.clear();
    file = fst.file_getAll(path_exe+"\\Archive\\orderList.txt");
    int cursor=0;
    int maxprint=22;
    bool running=true;
    std::string ch;
    std::string split[8];
    char num[3];
    char cursorPos[3];
    serial.write("\e[2J\e[0;0H\e[1;34;47m --- SAS file order inbox ---                                                  ");
    serial.write("\e[25;0H Arrow keys - Moving cursor | R - Report fulfilled order | E - Exit            \e[1;37;44m");

    while(running){
     file = fst.file_getAll(path_exe+"\\Archive\\orderList.txt");
     if(file.size()>0){
        if(maxprint>file.size()-1)maxprint=file.size()-1;
        serial.write("\e[2;0H");
        for(int i=0;i<=maxprint;i++){
                printf("drawing a new line\n");
            serial.print("");
            if(cursor==i){
                serial.write("\e[1;34;47m");
            }
            serial.write(file[i]);
            if(cursor==i){
                serial.write("\e[1;37;44m");
            }
        }
        while(true){
        ch=serial.getKey();
        if(ch=="\e[B"){//down
            if(cursor<maxprint){
                cursor++;
                break;
            }
        } else if(ch=="\e[A"){//up
            if(cursor>0){
                cursor=0;
                break;
            }
        } else if(ch=="r"){
            sprintf(cursorPos,"%d",cursor+3);
            serial.write("\e["+std::string(cursorPos)+";0H\e[1;34;47m - The file order has been fulfilled, an e-mail was sent to the orderer.\e[1;37;44m");
            file.erase(file.begin()+(cursor));
            fst.file_writeAll(path_exe+"\\Archive\\orderList.txt",file);

            /*
            tools.splitString(file[cursor],split);
            fileData=fst.file_getAll(path_exe+"\\Archive\\"+split[2]+'\\'+split[3]+'\\'+split[4]+'\\'+"data.txt");
            sprintf(num,"%d",atoi(fileData[8].c_str())+1);
            fileData[8]=std::string(num);
            fst.file_writeAll(path_exe+"\\Archive\\"+split[2]+'\\'+split[3]+'\\'+split[4]+'\\'+"data.txt",fileData,true);
            */
            serial.getKey();
            for(int i=0;i<=20;i++){
             sprintf(cursorPos,"%d",i+3);
             serial.write("\e["+std::string(cursorPos)+";0H\e[K");
            }
            cursor=0;
            break;
        } else if(ch=="e"){
            serial.write("\e[2J\e[0;0H");
            return;
            break;
        }
        }
     } else {
        serial.write("\e[3;0H\e[1;33m\a - There are no new requests for files.                                 \e[1;37m\e[25;0H\e[1;34;47m ANY - Exit |                                                      \e[1;37;44m");
        serial.getKey();
        serial.write("\e[2J\e[0;0H");
        return;
     }
    }

}

void SAS::menu(int clearance, int archivist, std::string uname){
    bool running = true;
    std::vector<std::string>fileData;
    std::string code;
    std::string attr;
    std::string val;
    bool attrValid,keepDrawing;
    std::string path[3];
    char num[3];
    serial.write("\e[1;37;44m\e[2J\e[0;0H");
    serial.print("  ______________");
    serial.print(" /___________  /\\");
    serial.print(" \\  ________/ / /");
    serial.print("  \\ \\ \\    / / /");
    serial.print("   \\ \\ \\  / / /");
    serial.print("    \\ \\ \\/ / /");
    serial.print("     \\ \\ \\/ /");
    serial.print("      \\ \\  /");
    serial.print("       \\_\\/\n");
    serial.print(" ~~~ Substructure Archive Seeking program ~~~\n");
    if(archivist>0)serial.print("Archival personnel registered!");
    while(running){
        fileData.clear();

        serial.write("Please enter or scan SAS-inventory code: \e[1;33m");
        code =serial.readLine(0);
        serial.write("\e[1;37m");
        tools.splitString(code,path,"-");
        if(path[0]=="exit"||path[0]=="EXIT"||path[0]=="e"||path[0]=="E"){running=false; return;};
        if(fs.dirExists(path_exe+"\\Archive\\"+path[0])){
            if(fs.dirExists(path_exe+"\\Archive\\"+path[0]+'\\'+path[1])){
                if(fs.dirExists(path_exe+"\\Archive\\"+path[0]+'\\'+path[1]+'\\'+path[2])){
                    keepDrawing=true;
                    while(keepDrawing){
                    fileData=fst.file_getAll(path_exe+"\\Archive\\"+path[0]+'\\'+path[1]+'\\'+path[2]+'\\'+"data.txt");
                    if(atoi(fileData[7].c_str())>=clearance){
                        serial.write("\e[2J\e[0;0H");
                        serial.print("\e[1;37mFile name:   \e[1;33m"+fileData[0]+"\e[1;39H\e[1;37m Location:   \e[1;33m"+fileData[5]);
                        serial.print("\e[1;37mDepartment:  \e[1;33m"+fileData[1]+"\e[2;39H\e[1;37m Project:    \e[1;33m"+fileData[2]);
                        serial.print("\e[1;37mCreated by:  \e[1;33m"+fileData[3]+"\e[3;39H\e[1;37m Created on: \e[1;33m"+fileData[4]);
                        serial.print("\e[1;37mAvailability: \e[1;33m"+fileData[8]+"\e[1;37m copies");
                        serial.print("Description: \e[1;33m"+fileData[6]);
                        if(archivist>0){
                            serial.write("\n\e[1;34;47m\e[12;0H E - exit | R - request file | D - edit data | -+ - change copies | ANY - ret   \e[1;37;44m\e[8;0H");
                        } else {
                            serial.write("\n\e[1;34;47m\e[12;0H E - exit | R - request file | ANY - return                                     \e[1;37;44m\e[8;0H");
                        }

                        switch(serial.getKey()[0]){
                            case 'e': running=false; return; break;
                            default: keepDrawing=false; break;
                            case 'r': if(atoi(fileData[8].c_str())>0){
                                         fst.file_append(path_exe+"\\Archive\\orderList.txt","@"+uname+" requests "+code+" at "+fileData[5].c_str());
                                         sprintf(num,"%d",atoi(fileData[8].c_str())-1);
                                         fileData[8]=std::string(num);
                                         fst.file_writeAll(path_exe+"\\Archive\\"+path[0]+'\\'+path[1]+'\\'+path[2]+'\\'+"data.txt",fileData,true);
                                         serial.print("\n\rA request has been sent to the Archive, you will receive an electronic mail\n\rwhen it is ready to be picked up.");
                                      } else {
                                         serial.print("\n\rThere is currently no copy available in the archive, please check back later.");
                                      }
                                      serial.write("Press any key to continue...");
                                      serial.getKey();
                                      break;
                            case 'd': if(archivist>0){
                                          serial.write("\n\r\e[1;33mWhich attribute do you wish to edit? Attributes numbererd in reading order: \e[1;37m");
                                          while(true){
                                            attr = serial.getKey();
                                            if(atoi(attr.c_str())>0&&atoi(attr.c_str())<=8){
                                                break;
                                            } else {
                                                serial.write("Invalid attribute, please submit valid attribute index number: ");
                                            }
                                          }
                                          serial.write("\n\r\e[1;33mPlease enter new value: \e[1;37m");
                                          attrValid=false;
                                          while(!attrValid){
                                            val = serial.readLine(0);
                                            switch(atoi(attr.c_str())){
                                                case 1: fileData[0]=val; attrValid=true; break;
                                                case 2: fileData[5]=val; attrValid=true; break;
                                                case 3: fileData[1]=val; attrValid=true; break;
                                                case 4: fileData[2]=val; attrValid=true; break;
                                                case 5: fileData[3]=val; attrValid=true; break;
                                                case 6: fileData[4]=val; attrValid=true; break;
                                                case 7: if(atoi(val.c_str())>-1&&atoi(val.c_str())<=99){
                                                            fileData[8]=val; attrValid=true; break;
                                                        } else {serial.write("\n\rInvalid number, please submit valid number: "); break;}
                                                case 8: fileData[6]=val; attrValid=true; break;
                                            }
                                          }
                                          fst.file_writeAll(path_exe+"\\Archive\\"+path[0]+'\\'+path[1]+'\\'+path[2]+'\\'+"data.txt",fileData,true);
                                          serial.write("Value updated successfully! Press any key to continue...");
                                          serial.getKey();
                                      } break;
                            case '-': if(atoi(fileData[8].c_str())>0&&archivist>0){
                                        sprintf(num,"%d",atoi(fileData[8].c_str())-1);
                                        fileData[8]=std::string(num);
                                        fst.file_writeAll(path_exe+"\\Archive\\"+path[0]+'\\'+path[1]+'\\'+path[2]+'\\'+"data.txt",fileData,true);
                                      } break;

                            case '+': if(archivist>0){
                                        sprintf(num,"%d",atoi(fileData[8].c_str())+1);
                                        fileData[8]=std::string(num);
                                        fst.file_writeAll(path_exe+"\\Archive\\"+path[0]+'\\'+path[1]+'\\'+path[2]+'\\'+"data.txt",fileData,true); break;
                                      }
                        }
                        serial.write("\e[2J\e[0;0H");
                    } else {
                        sTools.throwError(13);
                        keepDrawing=false;
                    }
                }
                } else {
                    serial.print("\n\rFile "+path[2]+" not found");
                }
            } else {
                serial.print("\n\rProject "+path[1]+" not found");
            }
        } else {
            serial.print("\n\rDepartment "+path[0]+" not found");
        }
    }
}
