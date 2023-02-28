#ifndef PARSERDEF_H
#define PARSERDEF_H
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#define HASH_MOD 1619
#define NUM_RULES 132

struct node
{
    int isTerminal;
    struct node* forward_link;
    struct node* backward_link;
    char value[23];
}; 

//both non-terminals and terminals
//non terminals = 69
char nonTerminals[127][24] = {"startprogram","program","moduleDeclarations","moduleDeclaration","otherModules",
"driverModule","module","ret","input_plist","moreList","output_plist","moreOutput",
"dataType","type","moduleDef","statements","statement","ioStmt","print_var","whichId2",
"boolvar","whichId","index","sign","aVar","var_id_num","newArithmeticExpr","startExpr",
"newA1","newA2","newTerm","newNextTerm","u1","after_u1","simpleStmt","assignmentStmt",
"whichStmt","lvalueIDStmt","lvalueARRStmt","moduleReuseStmt","optional","idList",
"moreId","actualParameter","expression","arithmeticOrBooleanExpr","ab1","anyTerm","ab2","u","after_unary",
"arithmeticExpr","a1","term","a2","nextTerm","op1","op2","bop","relationalOp",
"declareStmt","conditionalStmt","caseStmt","post","value","default","iterativeStmt",
"range_for","range","INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER",
"PROGRAM","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TAKES","INPUT","RETURNS","FOR","IN","SWITCH",
"CASE","BREAK","DEFAULT","WHILE","AND","OR","TRUE","FALSE","ID","NUM","RNUM","PLUS","MINUS","MUL","DIV",
"LT","LE","GE","GT","EQ","NE","DEF","ENDDEF","DRIVERDEF","DRIVERENDDEF","COLON","RANGEOP","SEMICOL",
"COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","COMMENTMARK","DOLLAR"};

char terminals[58][12] = {"INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER",
"PROGRAM","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TAKES","INPUT","RETURNS","FOR","IN","SWITCH",
"CASE","BREAK","DEFAULT","WHILE","AND","OR","TRUE","FALSE","ID","NUM","RNUM","PLUS","MINUS","MUL","DIV",
"LT","LE","GE","GT","EQ","NE","DEF","ENDDEF","DRIVERDEF","DRIVERENDDEF","COLON","RANGEOP","SEMICOL",
"COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","COMMENTMARK","DOLLAR"};

//stores lhs of rules
struct node* grammar[NUM_RULES];
//first and follow for all rules
char* firstAndFollow[NUM_RULES][NUM_RULES];
//parseTable
struct node* parseTable[HASH_MOD][HASH_MOD];
//
const char* comma = ",";
#endif