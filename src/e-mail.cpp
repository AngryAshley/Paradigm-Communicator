#include "e-mail.h"

e_mail::e_mail(){
    //this->path = path;
    //this->mailFolder = folder;
}

e_mail::~e_mail(){
}

void e_mail::refreshMailFolder(){
    mailFolder.clear();
    fs.read_dir_vect(path,mailFolder);
}

void e_mail::reset(){
    mailFolder.clear();
    path="";
}

int e_mail::getNewMail(bool verbose){
    std::string split[2];
    int amount = 0;

    refreshMailFolder();

    for(std::vector<int>::size_type i = 0; i != mailFolder.size();i++){
        tools.splitString(mailFolder[i],split,".");
        if(split[1] == "new"||split[1]=="NEW"){
            amount++;
        }
    }
    if(verbose){
    serial.write("You have ");
    if(amount>0){
        serial.write(std::to_string(amount));
    } else {
        serial.write("no");
    }
    serial.write(" new E-Mail");
    if(amount>1){
        serial.print("s");
    } else {
        serial.print("");
    }
    }
    return amount;
}

void e_mail::help(){
    serial.print("Paradigm's Electronic-Mail system. Enables you to read mail or send it to other users.");
    serial.print("");
    serial.print(" EMAIL CHECK - returns the amount of emails in your inbox.");
    serial.print(" EMAIL INBOX - Opens the e-mail inbox environment.");
    serial.print(" EMAIL HELP - Prints this help page.");
}


void e_mail::mail_sort(){
    std::string mailInfoTemp="";
    std::string mailInfo[1];
    std::vector<std::string> tempFolder;
    int sortDate[1024][2];
    std::string dateStr[5];
    int date[2];
    int dateInt;
    int sortedDate[1024][2];

    for(int i = 2; i != mailFolder.size();i++){
        mailInfoTemp = fstools.file_getLine(path+mailFolder[i],0);
        tools.splitString(mailInfoTemp,mailInfo,"|");
        tools.splitString(mailInfo[1],dateStr,"/- :");

        printf("\nDATE IN SEQUENCE\n");
        printf("\n%s",dateStr[0].c_str());
        printf("\n%s",dateStr[1].c_str());
        printf("\n%s",dateStr[2].c_str());
        printf("\n%s",dateStr[3].c_str());
        printf("\n%s",dateStr[4].c_str());

        ///dateInt = tools.dateToInt(date[0],date[1],date[2]);

        dateInt = tools.dateToInt(atoi(dateStr[2].c_str()),atoi(dateStr[1].c_str()),atoi(dateStr[0].c_str()),atoi(dateStr[3].c_str()),atoi(dateStr[4].c_str()));
        //printf("DateInt = %d\n",dateInt);
        sortDate[i][0]=i;
        sortDate[i][1]=dateInt;
        printf("\nPresort vector entry: %i | %i",sortDate[i][0],sortDate[i][1]);
    }
    return;
    //std::sort(std::begin(sortDate[1]), std::end(sortDate[1]), this->compareArray);


    int sorted = false;
    int smallestIndex = 0;
    int alreadySorted[1024];
    //int normalArray[];
    int arr_size = sizeof(sortDate);

    printf("\nBefore sort: ");
    for(int e=2; e<arr_size; e++){
        printf(" %d:",e);
        printf("%d",sortDate[e][1]);
    }

    while(sorted = false){
        int smallest = 9999;
        sorted = true;
        for(int i=0; i<arr_size;i++){
            int *result = std::find(alreadySorted, alreadySorted+1024, i);
            if(*result == 1024){
                if(sortDate[i][1]<smallest){
                    smallest = sortDate[i][1];
                    smallestIndex = i;
                    sorted = false;
                }
                sortedDate[i][0] = sortDate[smallestIndex][0];
                sortedDate[i][1] = smallest;
                alreadySorted[i] = smallestIndex;
            }
        }
    }

    printf("\nAfter sort: ");
    for(int i=0; i<arr_size; i++){
        printf("%n",sortedDate[i][1]);
    }

    /*
    std::qsort(std::begin(sortDate), 1024, sizeof(sortDate),
        [](const void *arg1, const void *arg2)->int
        {
            int const *lhs = static_cast<int const*>(arg1);
            int const *rhs = static_cast<int const*>(arg2);
            return (lhs[0] < rhs[0]) ? -1
                :  ((rhs[0] < lhs[0]) ? 1
                :  (lhs[1] < rhs[1] ? -1
                :  ((rhs[1] < lhs[1] ? 1 : 0))));
        });
    */

    tempFolder = mailFolder;
    for(std::vector<int>::size_type i = 2; i != tempFolder.size();i++){
        mailFolder[i]=tempFolder[sortedDate[i][0]];
    }
    printf("successfully sorted");

}

void e_mail::inbox_clearWindow(){
    serial.write("\e[4;0H");
    serial.write("\e[0m"+defColor);
    for(int i=0; i<20; i++){
        serial.print("\e[K");
    }
    serial.write("\e[4;0H");
}


int e_mail::inbox_drawContent(std::string file, int page){
    std::string file_path;
    int pageCount=0;
    int printIndex=0;
    file_path.std::string::append(this->path);
    file_path.std::string::append(file);
    printf(" file: %s",file_path.c_str());
    std::vector<std::string> content = fstools.file_getAll(file_path.c_str());
    serial.write("\e[4;0H"); ///1;32;40
    serial.write(defColor);

    //page++;
    int maxLines = 20;
    float tempPageCount = content.size()/maxLines;
    pageCount = ceil(tempPageCount)+1;
    int pageSize=content.size()-(page*maxLines);

    printf("\n\ncontentSize %d, PageCount %d, pageSize %d, page %d",content.size(),pageCount,pageSize,page);

    if(pageSize<maxLines){
        maxLines = pageSize;
        ///page = 1;
    }
    //page++;
    int iPre = 0;
    if(page==0){
        iPre = 1;
        //maxLines++;
        if(content.size()<22){
            maxLines=content.size();
        };
    } else {
        maxLines = content.size()-(21*page);   ///WORKED ON THIS LAST TIME!!! CHECK THIS!!!
    }
    for(int i=iPre; i<maxLines; i++){

        printIndex = i+(page*21); //maxLines
        serial.print(content[printIndex]); ///Try to align end of last page with begin of new page
        printf("%S",content[printIndex]);
    };
    return pageCount;
}

int e_mail::inbox_drawMail(int index, int page){
    std::string split[2];
    std::string mailInfoTemp = "";
    std::string mailInfo[1];
    int am = 0;
    //int linesToGo=23;
    int indexOffset = index+2;
    serial.write("\e[0m"+defColor);
    serial.write("\e[4;0H");
    serial.write(defColor);
    for(std::vector<int>::size_type i = 0; i != mailFolder.size();i++){
        tools.splitString(mailFolder[i].c_str(),split,".");
        if(split[1]=="new"||split[1]=="old"||split[1]=="NEW"||split[1]=="OLD"){
            if(indexOffset==i){
                serial.write(" ¯ \e[7m");
            } else {
                serial.write("\e[0m"+defColor+"   ");
            }
            am++;
            serial.write(split[0]);

            std::string command = "\e[" + std::to_string(am+3) + ";40H";
            serial.write(command);
            serial.write(split[1]);
            command = "\e[" + std::to_string(am+3) + ";47H";
            serial.write(command);

            mailInfoTemp = fstools.file_getLine(path+mailFolder[i],0);
            tools.splitString(mailInfoTemp,mailInfo,"|");

            serial.write(mailInfo[0]);
            command = "\e[" + std::to_string(am+3) + ";64H";
            serial.write(command);
            serial.write(mailInfo[1]);

            serial.print(defColor);
        }
        fill_n(split, 2, "");
        fill_n(mailInfo, 1, "");
        mailInfoTemp="";
    }
    serial.write("\e[24;0H");
    return am;
}

void e_mail::inbox_openMail(int index){
    std::string key;
    int pages=0;
    int pageIndex = 0;
    this->inbox_clearWindow();
    pages = this->inbox_drawContent(mailFolder[index+2].c_str(),pageIndex);
    serial.write("\e[24;0H\e[0m"+defColor+"\e[7m\e[K R = Reply | E = Exit");

    if(pages>0) serial.write(" | Arrow Page = page up/down");

    serial.write("\e[7m");

    printf("\n pages = %d, pageIndex = %d, index = %d", pages, pageIndex, index);

    while(true){
        key = serial.getKey();

        if(key=="\e[B"){
            if(pageIndex<pages-1){
                pageIndex++;
                this->inbox_clearWindow();
                pages = this->inbox_drawContent(mailFolder[index+2].c_str(),pageIndex);
            }
        } else if(key=="\e[A"){
            if(pageIndex>0){
                pageIndex--;
                this->inbox_clearWindow();
                pages = this->inbox_drawContent(mailFolder[index+2].c_str(),pageIndex);
            }
        } else if(key=="e"){
            break;
        };
    }
}

void e_mail::inbox(){
    std::string key;
    //int lines = 4;
    int index=0;
    int amount=0;
    int page=0;

    //this->mail_sort();

    serial.write("\e[0m");
    serial.write(defColor);
    serial.write("\e[2J");
    serial.write("\e[0;0H");
    serial.write("\e[7m\e[K PEMS - Paradigm Electronic Mail System                ");
    serial.write(tools.date());
    serial.print("\e[0m" + defColor);
               ///11111111111111111111111111111111111111112222222222222222222222222222222222222222   80-Columns
    serial.print(" Ä INBOX ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");
    amount = this->inbox_drawMail(index,page);
    serial.write("\e[7m\e[K Arrow keys = Select mail | Enter = Open | E = Exit | R = Refresh | S = Sort\e[0m");
    serial.write(defColor);

    while(true){
        key = serial.getKey();

        if(key=="\e[B"){
            if(index<amount-1){
                index++;
                amount = this->inbox_drawMail(index,page);
            }
        } else if(key=="\e[A"){
            if(index>0){
                index--;
                amount = this->inbox_drawMail(index,page);
            }
        } else if(key=="h"||key=="\r"){
            this->inbox_openMail(index);
            this->inbox_clearWindow();
            this->inbox_drawMail(index,page);
            serial.write("\e[7m\e[K Arrow keys = Select mail | Enter = Open | E = Exit | R = Refresh\e[0m");
            serial.write(defColor);
        } else if(key=="e"){
            serial.write("\e[2J\e[0;0H");
            break;
        } else if(key=="r"){
            this->inbox_clearWindow();
            this->refreshMailFolder();
            amount = this->inbox_drawMail(index,page);
        } else if(key=="s"){
            this->mail_sort();
            this->inbox_clearWindow();
            this->refreshMailFolder();
            amount = this->inbox_drawMail(index,page);
        };


    }
    serial.write("\e[2J\e[0m"+defColor);
}
