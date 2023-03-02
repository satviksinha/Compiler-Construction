#include "tree.h"

void addChild(struct treeNode* parent, struct node* child)
{
    struct treeNode* temp = malloc(sizeof(struct treeNode));
    strcpy(temp->value,child->value);
    temp->isTerminal = child->isTerminal;
    temp->parent = parent;
    temp->prevSibling = NULL;
    parent->children = temp;
    child = child->forward_link;
    
    while(child != NULL)
    {   
        struct treeNode* temp2 = malloc(sizeof(struct treeNode));
        strcpy(temp2->value,child->value);
        if(!strcmp(child->value,"DOLLAR"))
            temp2->children = NULL;
        temp2->isTerminal = child->isTerminal;
        temp2->parent = parent;
        temp2->prevSibling = temp;
        temp->nextSibling = temp2;
        temp = temp2;
        child = child->forward_link;
    }
    temp->nextSibling = NULL;
}


