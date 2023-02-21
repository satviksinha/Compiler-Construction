#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

int get_hash(const char* s);

struct node
{
    int isTerminal;
    struct node* link;
    char* value;
}; 


struct node* grammar[128];
// Doubt - how to declare array?
char** firstAndFollow[128];
struct node* parseTable[100][100];

// function for storing grammar rules in the form of linked list
void makeGrammar(FILE* fp);

void createParseTable();
