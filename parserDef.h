#ifndef PARSERDEF_H
#define PARSERDEF_H
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>


int get_hash_nonTerminal(const char* s);
int get_hash_terminal(const char *s);

struct node
{
    int isTerminal;
    struct node* forward_link;
    struct node* backward_link;
    char value[23];
}; 

char nonTerminals[67][24] = {"startprogram","program","moduleDeclarations","otherModules",
"driverModule","module","ret","inputp_list","moreList","outputp_list","moreOutput",
"dataType","type","moduleDef","statements","statement","ioStmt","print_var","whichId2",
"boolvar","whichId","index","sign","aVar","var_id_num","newArithmeticExpr","startExpr",
"newA1","newA2","newTerm","newNextTerm","u1","after_u1","simpleStmt","asssignmentStmt",
"whichStmt","lvalueIDStmt","lvalueARRStmt","moduleReuseStmt","optional","idList",
"moreId","expression","arithmeticOrBooleanExpr","ab1","anyTerm","ab2","u","after_unary",
"arithmeticExpr","a1","term","a2","nextTerm","op1","op2","bop","relationalOp",
"declareStmt","conditionalStmt","caseStmt","post","value","default","iterativeStmt",
"range_for","range"};

char terminals[57][13] = {"INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER",
"PROGRAM","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TAKES","INPUT","RETURNS","FOR","IN","SWITCH",
"CASE","BREAK","DEFAULT","WHILE","AND","OR","TRUE","FALSE","ID","NUM","RNUM","PLUS","MINUS","MUL","DIV",
"LT","LE","GE","GT","EQ","NE","DEF","ENDDEF","DRIVERDEF","DRIVERENDDEF","COLON","RANGEOP","SEMICOL",
"COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","COMMENTMARK"};


struct node* grammar[128];
// Doubt - how to declare array?
char** firstAndFollow[128];
struct node* parseTable[100][100];
#endif