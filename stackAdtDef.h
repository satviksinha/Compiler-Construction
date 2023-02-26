#ifndef STACKADTDEF_H
#define STACKADTDEF_H

#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>


struct stack_node
{
    int isTerminal;
    char value[23];
    struct stack_node* link;
};

typedef struct stack_node stackElement;

stackElement* s_top = NULL;

#endif