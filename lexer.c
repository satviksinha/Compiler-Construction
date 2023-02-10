#include"lexer.h"

void getnextblock(FILE* fp,char * buff){
    fread(buff,512,1,fp);
}

char getnextchar(FILE *fp,char *buff1,char *buff2){
    switch(flag){
        case 0:
        if(forward == 512){
            getnextblock(fp,buff2);
            forward = 0;
            flag = 1;
            return buff2[forward++]; 
        }
        return buff1[forward++];
        break;
        case 1:
        return buff2[forward++];
        break;
        case 2:
        return buff1[forward++];
        break;
        case 3:
        if(forward == 512){
            getnextblock(fp,buff1);
            forward = 0;
            flag = 2;
            return buff1[forward++]; 
        }
        return buff2[forward++];
        break;
    }
}

int main(){
    FILE* fp = fopen("testcase.txt","r");
    getnextblock(fp,buff1);
    return 0;
}