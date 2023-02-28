#include <stdio.h>

int main(){
    FILE *fp = fopen("testcase.txt","r");
     if(fp == NULL){
        printf("File not found");
        return 0;
    }
    int flag = 0;
    while(!feof(fp)){
        char c = fgetc(fp);
        int flag2=0;
        if(c=='*' && flag == 0){
            char c = fgetc(fp);
            if(c=='*'){
                flag =1;
            }
        fseek(fp, -1, SEEK_CUR);
        }
        else if(c=='*' && flag==1){
            char c = fgetc(fp);
            if(c=='*'){
                flag=0;
                printf("  ");
                flag2=1;
            }
            else fseek(fp, -1, SEEK_CUR);

        }
        if(!flag2){
        if(flag==0)printf("%c",c);
        else printf(" ");
        }
        
    }
    
    return 0;
}