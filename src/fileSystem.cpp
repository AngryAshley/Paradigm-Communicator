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
