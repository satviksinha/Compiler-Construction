#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

int get_hash(const char* s);

struct node
{
    int isTerminal;
    struct node* link;
    char value[23];
}; 

// char nonTerminals[67][23] = {"startprogram","program","moduleDeclarations","otherModules",
// "driverModule","module","ret","inputp_list","moreList","outputp_list","moreOutput",
// "dataType","type","moduleDef","statements","statement","ioStmt","print_var","whichId2",
// "boolvar","whichId","index","sign","aVar","var_id_num","newArithmeticExpr","startExpr",
// "newA1","newA2","newTerm","newNextTerm","u1","after_u1","simpleStmt","asssignmentStmt",
// "whichStmt","lvalueIDStmt","lvalueARRStmt","moduleReuseStmt","optional","idList",
// "moreId","expression","arithmeticOrBooleanExpr","ab1","anyTerm","ab2","u","after_unary",
// "arithmeticExpr","a1","term","a2","nextTerm","op1","op2","bop","relationalOp",
// "declareStmt","conditionalStmt","caseStmt","post","value","default","iterativeStmt",
// "range_for","range"};

//both non-terminals and terminals
//non terminals = 68
char nonTerminals[126][24] = {"startprogram","program","moduleDeclarations","moduleDeclaration","otherModules",
"driverModule","module","ret","input_plist","moreList","output_plist","moreOutput",
"dataType","type","moduleDef","statements","statement","ioStmt","print_var","whichId2",
"boolvar","whichId","index","sign","aVar","var_id_num","newArithmeticExpr","startExpr",
"newA1","newA2","newTerm","newNextTerm","u1","after_u1","simpleStmt","assignmentStmt",
"whichStmt","lvalueIDStmt","lvalueARRStmt","moduleReuseStmt","optional","idList",
"moreId","expression","arithmeticOrBooleanExpr","ab1","anyTerm","ab2","u","after_unary",
"arithmeticExpr","a1","term","a2","nextTerm","op1","op2","bop","relationalOp",
"declareStmt","conditionalStmt","caseStmt","post","value","default","iterativeStmt",
"range_for","range","INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER",
"PROGRAM","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TAKES","INPUT","RETURNS","FOR","IN","SWITCH",
"CASE","BREAK","DEFAULT","WHILE","AND","OR","TRUE","FALSE","ID","NUM","RNUM","PLUS","MINUS","MUL","DIV",
"LT","LE","GE","GT","EQ","NE","DEF","ENDDEF","DRIVERDEF","DRIVERENDDEF","COLON","RANGEOP","SEMICOL",
"COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","COMMENTMARK","DOLLAR"};

char terminals[59][12] = {"INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER",
"PROGRAM","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TAKES","INPUT","RETURNS","FOR","IN","SWITCH",
"CASE","BREAK","DEFAULT","WHILE","AND","OR","TRUE","FALSE","ID","NUM","RNUM","PLUS","MINUS","MUL","DIV",
"LT","LE","GE","GT","EQ","NE","DEF","ENDDEF","DRIVERDEF","DRIVERENDDEF","COLON","RANGEOP","SEMICOL",
"COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","COMMENTMARK","DOLLAR","EPSILON"};


struct node* grammar[129];
// Doubt - how to declare array?
char* firstAndFollow[129][111];
struct node* parseTable[1519][1519];

// function for storing grammar rules in the form of linked list
void makeGrammar(FILE* fp);
// function for populating parse table with grammar rules
void createParseTable();
//gives first and follow for each rule
void computeFirstAndFollow();
//removes duplicates in follow sets
void removeDuplicates();




