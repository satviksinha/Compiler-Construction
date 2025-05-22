/*
ID: 2020A7PS0297P                             NAME: Sarang Sridhar 
ID: 2020A7PS0995P                             NAME: Kashish Mahajan 
ID: 2020A7PS0993P                             NAME: Satvik Sinha 
ID: 2020A7PS0036P                             NAME: Aarya Attrey
ID: 2020A7PS0017P                             NAME: Urvashi Sharma 
*/
#ifndef TREEDEF_H
#define TREEDEF_H

#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

struct treeNode
{
    int line_no;
    char value[23]; //token name
    int isTerminal;
    struct treeNode* children;
    struct treeNode* parent;
    struct treeNode* nextSibling;
    struct treeNode* prevSibling;
    union Variant tk_data;
};

struct treeNode* root = NULL;
struct treeNode* currExpand = NULL;
#endif