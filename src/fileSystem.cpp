#include "fileSystem.h"

fileSystem::fileSystem()
{
    //ctor
}

fileSystem::~fileSystem()
{
    //dtor
}

void fileSystem::showDir(){
    std::string pattern(cd);
    std::string path;
    int filesFound = 0;
    pattern.append("\\*");
    FILETIME ft;
    WIN32_FIND_DATA data;
    HANDLE hFind;
    if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
        do {
            path = cd + "\\";
            path.append(data.cFileName);
            DWORD attr = GetFileAttributes(path.c_str());
            printf("Attr %d", attr);
            if((attr & FILE_ATTRIBUTE_HIDDEN)==FILE_ATTRIBUTE_HIDDEN){
            } else {
            filesFound++;

            SYSTEMTIME stUTC;
            FileTimeToSystemTime(&data.ftLastWriteTime, &stUTC);
            char timestr [50];
            sprintf(timestr,"%02d/%02d/%d %02d:%02d", stUTC.wDay, stUTC.wMonth, stUTC.wYear, stUTC.wHour, stUTC.wMinute);

            serial.write(" "+std::string(data.cFileName));
            if(FILE_ATTRIBUTE_DIRECTORY == attr){
                serial.write("\e[40D\e[15C <DIR>");
            }
            serial.print("\e[40D\e[22C"+std::string(timestr));
            }
        } while (FindNextFile(hFind, &data) != 0);
        FindClose(hFind);
    }
    if(filesFound<=2){
        serial.print("\n\r This directory is empty ");
    }
}

void fileSystem::changeDir(std::string dir){
    if(dir==".."){
        printf("changing path %s",cd.c_str());
        std::vector<int> pos = this->findLocation(cd,'\\');
        int offset = pos[pos.size()-2];
        cd.erase(cd.begin() + offset, cd.end());
    } else if(dir=="."){
        return;
    } else if(this->dirExists(cd+"\\"+dir)){
        cd.append("\\");
        cd.append(dir);
        cd.append("\\");
        printf("changing path %s",cd.c_str());
    } else {
        serial.print("No such directory");
    }
}

void fileSystem::printBigFile(std::string path){
    std::vector<std::string> file = fst.file_getAll(path);
    int lines=file.size();
    int maxLines=23;
    int linesLeft=0;
    int bigfile=true;
    if(lines<maxLines){
        maxLines=lines;
        bigfile=false;
    }
    serial.write("\e[0m"+defColor);
    //serial.write(defColor);
    for(int i=0; i<lines; ){
        linesLeft=lines-i;
        if(linesLeft<maxLines){
            maxLines=linesLeft;
        }
        for(int j=0; j<maxLines; j++){
            serial.print(file[i]);
            i++;
        }
        if(linesLeft>maxLines){
            serial.write("\e[7m\e[K Press PgDn or arrow-down to continue, any other key to exit \e[0m"+defColor);
            serial.write(defColor);
            key = serial.getKey();
            printf("key=%s", key.c_str());
            if(key=="\e[6~"||key=="\e[B"){
                serial.write("\e[80D\e[K");
                continue;
            } else {
                serial.write("\e[80D\e[K");
                break;
            }
        }
    }
}


bool fileSystem::dirExists(const std::string& dirName_in){
  DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with your path!

  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;   // this is a directory!

  return false;    // this is not a directory!
}
bool fileSystem::fileExists(const std::string& fileName){
  DWORD ftyp = GetFileAttributesA(fileName.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with your path!

  return true;    // this is not a directory!
}
void fileSystem::hideFile(const std::string& fileName){
    DWORD ftyp = GetFileAttributesA(fileName.c_str());
   if ((ftyp & FILE_ATTRIBUTE_HIDDEN) == 0) {
       SetFileAttributes(fileName.c_str(), ftyp | FILE_ATTRIBUTE_HIDDEN);
    }
}

std::vector<int> fileSystem::findLocation(std::string sample, char findIt){
    std::vector<int> characterLocations;
    for(int i =0; i < sample.size(); i++)
        if(sample[i] == findIt)
            characterLocations.push_back(i);

    return characterLocations;
}

void fileSystem::read_dir_vect(const std::string& name, std::vector<std::string>& v){
    std::string pattern(name);
    //int i = 0;
    pattern.append("\\*");
    WIN32_FIND_DATA data;
    HANDLE hFind;
    if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
        do {
            //dir_arr[i] = data.cFileName;
            v.push_back(data.cFileName);
            //i++;
        } while (FindNextFile(hFind, &data) != 0);
        FindClose(hFind);
    }
}

std::string fileSystem::getPath(){
char path_exe_temp[1024];
GetModuleFileName(NULL, path_exe_temp, 1024);
int pos=std::string(path_exe_temp).find_last_of("\\/");
return std::string(path_exe_temp).substr( 0, pos+1);
}

int fileSystem::DeleteDirectory(const std::string &refcstrRootDirectory, bool bDeleteSubdirectories){
  bool            bSubdirectory = false;       // Flag, indicating whether
                                               // subdirectories have been found
  HANDLE          hFile;                       // Handle to directory
  std::string     strFilePath;                 // Filepath
  std::string     strPattern;                  // Pattern
  WIN32_FIND_DATA FileInformation;             // File information


  strPattern = refcstrRootDirectory + "\\*.*";
  hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
  if(hFile != INVALID_HANDLE_VALUE)
  {
    do
    {
      if(FileInformation.cFileName[0] != '.')
      {
        strFilePath.erase();
        strFilePath = refcstrRootDirectory + "\\" + FileInformation.cFileName;

        if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
          if(bDeleteSubdirectories)
          {
            // Delete subdirectory
            int iRC = DeleteDirectory(strFilePath, bDeleteSubdirectories);
            if(iRC)
              return iRC;
          }
          else
            bSubdirectory = true;
        }
        else
        {
          // Set file attributes
          if(::SetFileAttributes(strFilePath.c_str(),
                                 FILE_ATTRIBUTE_NORMAL) == FALSE)
            return ::GetLastError();

          // Delete file
          if(::DeleteFile(strFilePath.c_str()) == FALSE)
            return ::GetLastError();
        }
      }
    } while(::FindNextFile(hFile, &FileInformation) == TRUE);

    // Close handle
    ::FindClose(hFile);

    DWORD dwError = ::GetLastError();
    if(dwError != ERROR_NO_MORE_FILES)
      return dwError;
    else
    {
      if(!bSubdirectory)
      {
        // Set directory attributes
        if(::SetFileAttributes(refcstrRootDirectory.c_str(),
                               FILE_ATTRIBUTE_NORMAL) == FALSE)
          return ::GetLastError();

        // Delete directory
        if(::RemoveDirectory(refcstrRootDirectory.c_str()) == FALSE)
          return ::GetLastError();
      }
    }
  }

  return 0;
}

void fileSystem::db(std::string dbID, int clearance){
    std::string path = fileSystem::getPath();
    if(!fileSystem::fileExists(path+"\\DB\\"+dbID+".TXT")){
        serial.print(" - Database \""+dbID+"\" not found.");
        return;
    }
    std::string metadataTemp = fst.file_getLine(path+"\\DB\\"+dbID+".TXT",0);
    std::string metadata[32];
    int dataLengths[16];
    int mdataLen=0;
    int dbClearance = 0;
    tools.splitString(metadataTemp,metadata,".|");
    for(int i=0; i<32; i++){
        if(metadata[i]=="#CLEVEL"){
            dbClearance=atoi(metadata[i+1].c_str());
            break;
        }
    }
    if(clearance<dbClearance){
        serial.print(" - You are not authorized to access this database.");
        return;
    }
    for(int i=0;i<32;i++){
        if(metadata[i]==""){
            mdataLen=i;
            break;
        }
    }

    serial.write("\e[2J\e[0;0H");
    int delim=1;
    int j=1;
    dataLengths[0]=delim;
    serial.print(defColor+"\e[47m\e[1;34m\e[KDatabase - "+dbID+defColor);
    serial.write("\e[24;0H\e[47m\e[1;34m\e[K E Exit | S Sort | F Filter | M Modify | D Delete | N New | v Next | ^ Previous |"+defColor);
    for(int i=0;i<mdataLen;i++){
     if(metadata[i][0]=='#'){
        i++;
        continue;
     }

     serial.write("\e[2;"+std::to_string(delim)+"H"+metadata[i]);
     delim+=atoi(metadata[i+1].c_str())+1;
     dataLengths[j]=delim;
     j++;
     i++;
    }
    serial.print("\n\r-------------------------------------------------------------------------------");
    std::string currentLine = "";
    std::string currentLineS[16];
    int nextLine=1;
    int offset=0;
    int selected=0;
    int selectPos=0;
    int filtered=0;
    std::string key="";
    std::vector<std::string> file;
    file = fst.file_getAll(path+"\\DB\\"+dbID+".TXT");
    while(true){
        nextLine=1;
        while(true){
            currentLine=fst.file_getLine(path+"\\DB\\"+dbID+".TXT",nextLine+offset);
            if(currentLine!=""){
                tools.splitString(currentLine,currentLineS,"|");
                if((nextLine-1)+offset==selected){
                    serial.write("\e["+std::to_string(nextLine+3)+";0H");
                    serial.write("\e[47m\e[1;34m\e[K");
                } else {
                    serial.write("\e["+std::to_string(nextLine+3)+";0H");
                    serial.write(defColor+"\e[K");
                }

                for(int k=0;k<j;k++){
                    serial.write("\e["+std::to_string(nextLine+3)+";"+std::to_string(dataLengths[k])+"H"+currentLineS[k]);
                }
                serial.write(defColor);
            } else {
                break;
            }
            nextLine++;
            if(nextLine==21){
                break;
            }
        }
        key = serial.getKey();

        if(key=="e"){
            serial.write(defColor+"\e[2J\e[0;0H");
            break;
        }
        if(key=="\e[A"){
            if(selectPos==0&&selected>0){
                offset--;
            }
            if(selected>0){
                selected--;
                if(selectPos>0){
                    selectPos--;
                }
            } else {
                if(offset>0){
                    offset--;
                    selected--;
                }
            }
        }
        if(key=="\e[B"){
            if(selected+offset<=file.size()){
                if(selectPos==19){
                    offset++;
                    selected++;
                } else {
                    if(selectPos<19){
                        selectPos++;
                    }
                    selected++;
                }
            }
        }

    }




    return;
}

int fileSystem::fileEditor_drawMenu(int mode, int select1, int select2){
    if(mode==0){
        serial.write("\e[2J"+defColor+"\e[0;0H");
        serial.print("\e[7m\e[K\e["+std::to_string(40-(fe_currentFile.std::string::size()/2))+"C"+fe_currentFile);
        serial.write("\e[K   File      Settings ");
        serial.write("\e[24;0H\e[K Press F1 for menu\e[2;0H\e[0m"+this->defColor);
        return 0;
    }
    if(mode==1){
        //serial.write("\e[3;0H\e[K\n\e[K\n\e[K\n\e[K\n\e[K\n\e[K\n\e[K");
        serial.write("\e[2;0H");
        switch(select1){
            case 0:
                serial.write("\e[46m  >File<  "+this->defColor+"\e[7m   Settings \e[0m"+this->defColor);
                serial.write("\e[3;1H\e[47m  New     \e[40m \n");
                serial.write("\e[4;1H\e[47m  Save    \e[40m \n");
                serial.write("\e[5;1H\e[47m  print   \e[40m \n");
                serial.write("\e[6;1H\e[47m  Dwnload \e[40m \n");
                serial.write("\e[7;1H\e[47m  Exit    \e[40m \n");
                serial.write("\e[8;2H\e[40m          ");
                switch(select2){
                    case 0: serial.write("\e[3;1H\e[1;33;47m> New     \e[1;34;40m \n"); break;
                    case 1: serial.write("\e[4;1H\e[1;33;47m> Save    \e[1;34;40m \n"); break;
                    case 2: serial.write("\e[5;1H\e[1;33;47m> print   \e[1;34;40m \n"); break;
                    case 3: serial.write("\e[6;1H\e[1;33;47m> Dwnload \e[1;34;40m \n"); break;
                    case 4: serial.write("\e[7;1H\e[1;33;47m> Exit    \e[1;34;40m \n"); break;
                }
                break;
            case 1:
                serial.write(this->defColor+"\e[7m   File   \e[0m"+this->defColor+"\e[46m  >Settings<  \e[0m"+this->defColor);
                serial.write("\e[3;11H\e[47m  Null    \e[40m \n");
                serial.write("\e[4;11H\e[47m  Null    \e[40m \n");
                serial.write("\e[5;11H\e[47m  Null    \e[40m \n");
                serial.write("\e[6;11H\e[47m  Null    \e[40m \n");
                serial.write("\e[7;11H\e[47m  Null    \e[40m \n");
                serial.write("\e[8;11H\e[47m  Null    \e[40m \n");
                serial.write("\e[9;12H\e[40m          ");
                switch(select2){
                    case 0: serial.write("\e[3;11H\e[1;33;47m> Null    \e[1;34;40m \n"); break;
                    case 1: serial.write("\e[4;11H\e[1;33;47m> Null    \e[1;34;40m \n"); break;
                    case 2: serial.write("\e[5;11H\e[1;33;47m> Null    \e[1;34;40m \n"); break;
                    case 3: serial.write("\e[6;11H\e[1;33;47m> Null    \e[1;34;40m \n"); break;
                    case 4: serial.write("\e[7;11H\e[1;33;47m> Null    \e[1;34;40m \n"); break;
                    case 5: serial.write("\e[8;11H\e[1;33;47m> Null    \e[1;34;40m \n"); break;
                }
                break;
        }
        return 0;
    }
    if(mode==2){
        switch(select1){
            case 0: return 4;
            case 1: return 5;
        }
    }
}

int fileSystem::fileEditor(std::string path){
    stools.serial = serial;
    std::vector<std::string> file;
    int curLine=0;
    int curCol=0;
    char currentLine[80];
    bool inMenu = false;
    int sel1=0;
    int sel2=0;
    bool editing=true;

    if(this->fileExists(cd+"\\"+path)==false || path=="new"){
            serial.print("File Editor:");
        if(stools.choice(path+" doesn't exist, create new file?")==1){return 0;};
        this->fe_currentFile="New.file";
    } else {
        this->fe_currentFile=path;
    }
    this->fileEditor_drawMenu();
    if(fe_currentFile!="New.file"){  ///load file if file not new(aka no file to read);
       file = fst.file_getAll(cd+fe_currentFile);
    }
    int fileLen=file.size();
    int maxLines=21;
    if(fileLen<maxLines)maxLines=fileLen;
    strcpy(currentLine,file[curLine].c_str());
    serial.write("\e[3;0H");
    for(int i=0; i<maxLines; i++){
        serial.write(file[i]);
        serial.write("\e["+std::to_string(4+i)+";0H");
    }

    while(editing){
        serial.write("\e["+std::to_string(curLine+3)+";"+std::to_string(curCol)+"H");
        std::string key = serial.getKey();
        if(key.std::string::size()>1 || key=="\x0d" || key=="\x08"||key=="\x1b"){
            if(key=="\x0d"){
                currentLine[curCol]=key[0];
                file[curLine]=currentLine;
                curCol++;
                curLine++;
                strcpy(currentLine,file[curLine].c_str());

            }
            if(key=="\x08"){
                file[curLine].std::string::erase(curCol,1);
            }
            if(key=="\e[A"){   ///keys in sequence UP DOWN RIGHT LEFT
                if(curLine!=0){
                    curLine--;
                    strcpy(currentLine,file[curLine].c_str());
                }
            }
            if(key=="\e[B"){
                if(curLine!=21){
                    curLine++;
                    strcpy(currentLine,file[curLine].c_str());
                }
            }
            if(key=="\e[C"){
                if(curCol!=79){curCol++;}
            }
            if(key=="\e[D"){
                if(curCol!=0){curCol--;}
            }
            if(key=="\eOP"){
                inMenu=true;
                sel1=0;
                sel2=0;
                while(inMenu){
                    this->fileEditor_drawMenu(1,sel1,sel2);
                    std::string key = serial.getKey();
                    printf("%s",key);
                    if(key.std::string::size()>1 || key=="\x0d" || key=="\x08"||key=="\x1b"){
                        if(key=="\e[A"){   ///keys in sequence UP DOWN RIGHT LEFT
                            if(sel2!=0){sel2--;}
                        }
                        if(key=="\e[B"){
                            if(sel2!=fileEditor_drawMenu(2,sel1)){sel2++;}
                        }
                        if(key=="\e[C"){
                            if(sel1!=1){sel1++;}
                        }
                        if(key=="\e[D"){
                            if(sel1!=0){sel1--;}
                        }
                        if(key=="\eOP"){
                            inMenu=false;
                            serial.write("\e[0m"+this->defColor);
                            fileEditor_drawMenu(0);
                        }
                        if(key=="\x0d"){
                                printf("Selected option %d, %d",sel1, sel2);
                            switch(sel1){
                                case 0: switch(sel2){
                                            case 4: editing=false; inMenu=false; serial.write(this->defColor+"\e[2J\e[0;0H"); return 1;


                                            }
                                            break;


                            }
                        }
                    } else {
                        switch(key[0]){}
                    }
                }
            }
        } else {
            currentLine[curCol]=key[0];
            curCol++;
        }
        serial.write("\e["+std::to_string(curLine+3)+";0H"+currentLine);
    }
}
