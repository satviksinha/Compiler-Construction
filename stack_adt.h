#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>


struct node
{
    int isTerminal;
    char* value;
    struct node* link;
};



typedef struct node stackElement;

stackElement* top = NULL;

//stack functions
void push(stackElement *element);
void pop();
int isEmpty();



