#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

//Token names
enum TOKEN {INTEGER,REAL,BOOLEAN,OF,ARRAY,START,END,DECLARE,MODULE,DRIVER,
PROGRAM,GET_VALUE,PRINT,USE,WITH,PARAMETERS,TAKES,INPUT,RETURNS,FOR,IN,SWITCH,
CASE,BREAK,DEFAULT,WHILE,AND,OR,TRUE,FALSE,ID,NUM,RNUM,PLUS,MINUS,MUL,DIV,
LT,LE,GE,GT,EQ,NE,DEF,ENDDEF,DRIVERDEF,DRIVERENDDEF,COLON,RANGEOP,SEMICOL,
COMMA,ASSIGNOP,SQBO,SQBC,BO,BC,COMMENTMARK};


int token_found=0;
// union
union Variant{
    char lexeme[21];
    int val;
    double realVal;
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
#define buffer_size 30
char buff1[buffer_size];
char buff2[buffer_size];

// pointers to the buffer
int forward=0,begin=0;
 /*0: both pointers in buff1
 1: begin in buff1 and forward in buff2
 2: forward in buff1 and begin in buff2
 3: both in buff2*/
int flag = 0;

// stores current_line_no
int current_line_no=1;

//global variable for state for dfa
int state = 0;

//hashtable
struct ht_entry{
    enum TOKEN tk;
    char str[11];
};

struct ht_entry hashTable[84];

//function prototypes

// tk_data has to be set according to tk_name
void tokenise(enum TOKEN tk_name);

// get the next character
char getnextchar(FILE *fp,char * buff1,char * buff2);

// populate the buffer
void getnextblock(FILE* fp,char * buff);

//check for keywords,AND,OR,true,false
int check_keyword(char *lexeme);

//error handling function
void error_handle();

//copying lexeme
void copy_lexeme(char * str);

//get the hash value for a string
int get_hash(char* str);
