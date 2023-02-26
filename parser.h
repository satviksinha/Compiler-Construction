#ifndef PARSER_H
#define PARSER_H

#include "parserDef.h"

// function for storing grammar rules in the form of linked list
void makeGrammar(FILE* fp);


//function for parse table generation
void createParseTable();

// passing token to parser
void getNextToken();

//parsing using stack
void runPDA();

//error handling
void display_error();

#endif

