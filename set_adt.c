#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SET_SIZE 50
#define MAX_FOLLOW_SIZE 120

char set[MAX_SET_SIZE][MAX_FOLLOW_SIZE];

int push(char* str,char* push_to){      //push_to will be set[i], str will be string to be pushed
    if(!strlen(push_to)){
        strcat(push_to,str);
        return 1;
    }
    char* temp;
    strcpy(temp,push_to);
    char* token = strtok(temp,",");
    while(token!=NULL){
        if(!strcmp(token,str)){
            return 0;
        }
        token = strtok(NULL,",");
    }

    strcat(push_to,",");
    strcat(push_to,str);
    return 1;
}

int main(){
    push("abc",set[0]);
    push("def",set[0]);
    printf("\n %s ",set[0]);
    push("def",set[0]);
    printf("\n %s ",set[0]);
}


