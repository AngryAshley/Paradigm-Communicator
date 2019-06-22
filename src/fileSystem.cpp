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
    int filesFound = 0;
    pattern.append("\\*");
    FILETIME ft;
    WIN32_FIND_DATA data;
    HANDLE hFind;
    if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
        do {
            filesFound++;
            std::string path = cd + "\\";
            path.append(data.cFileName);
            SYSTEMTIME stUTC;

            DWORD attr = GetFileAttributes(path.c_str());

            FileTimeToSystemTime(&data.ftLastWriteTime, &stUTC);
            //printf(" %s %d ", path.c_str(), attr);
            serial.write(" ");
            serial.write(data.cFileName);
            if(FILE_ATTRIBUTE_DIRECTORY == attr){
                serial.write("\e[40D\e[15C <DIR>");
            }
            serial.write("\e[40D\e[22C");
            char timestr [50];
            sprintf(timestr,"%02d/%02d/%d %02d:%02d", stUTC.wDay, stUTC.wMonth, stUTC.wYear, stUTC.wHour, stUTC.wMinute);
            serial.print(timestr);

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
        serial.print("No such file or directory");
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
    serial.write("\e[0m");
    serial.write(defColor);
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
            serial.write("\e[7m\e[K Press PgDn to continue, any other key to exit \e[0m");
            serial.write(defColor);
            key = serial.getKey();
            printf("key=%s", key.c_str());
            if(key=="\e[6~"){
                serial.write("\e[50D\e[K");
                continue;
            } else {
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

std::vector<int> fileSystem::findLocation(std::string sample, char findIt){
    std::vector<int> characterLocations;
    for(int i =0; i < sample.size(); i++)
        if(sample[i] == findIt)
            characterLocations.push_back(i);

    return characterLocations;
}

