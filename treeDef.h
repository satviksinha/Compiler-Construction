#ifndef TREEDEF_H
#define TREEDEF_H

#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

struct treeNode
{
    // int line_no;
    char value[23];
    int isTerminal;
    struct treeNode* children;
    struct treeNode* parent;
    struct treeNode* nextSibling;
    struct treeNode* prevSibling;
};

struct treeNode* root = NULL;
struct treeNode* currExpand = NULL;
#endif