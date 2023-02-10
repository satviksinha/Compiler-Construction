#include<stdio.h>
#include<stdlib.h>

//Token names
enum TOKEN {INTEGER,REAL,BOOLEAN,OF,ARRAY,START,END,DECLARE,MODULE,DRIVER,
PROGRAM,GET_VALUE,PRINT,USE,WITH,PARAMETERS,TAKES,INPUT,RETURNS,FOR,IN,SWITCH,
CASE,BREAK,DEFAULT,WHILE,ID,NUM,RNUM,AND,OR,TRUE,FALSE,PLUS,MINUS,MUL,DIV,
LT,LE,GE,GT,EQ,NE,DEF,ENDDEF,DRIVERDEF,DRIVERENDDEF,COLON,RANGEOP,SEMICOL,
COMMA,ASSIGNOP,SQBO,SQBC,BO,BC,COMMENTMARK};

// union
union Variant{
    char lexeme[21];
    int val;
    float realVal;
};

//struct
typedef struct token_info{
    int line_no;
    enum TOKEN tk_name;
    union Variant tk_data;
} TK_INFO;

//global declarations

//global token
TK_INFO global_token;

// twin buffers
char buff1[512];
char buff2[512];

// pointers to the buffer
int forward=0,begin=0;

 /*0: both pointers in buff1
 1: begin in buff1 and forward in buff2
 2: forward in buff1 and begin in buff2
 3: both in buff2*/
int flag = 0;

//function prototypes

// tk_data has to be set according to tk_name
void tokenise(int line_no,enum TOKEN tk_name,char * tk_data);

// get the next character
char getnextchar(FILE *fp,char * buff1,char * buff2);

// populate the buffer
void getnextblock(FILE* fp,char * buff);

//check for keywords,AND,OR,true,false
int check_keyword(char *lexeme);
