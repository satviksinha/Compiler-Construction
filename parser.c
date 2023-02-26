#include "parserDef.h"
#include "lexerDef.h"
#include "tree.h"

struct stack_node
{
    int isTerminal;
    char value[23];
    struct stack_node* link;
};

typedef struct stack_node stackElement;

stackElement* s_top = NULL;

void s_push(stackElement* element)
{
    //creating back link
    element->link = s_top;
    
    // change top
    s_top  = element;
}

void s_pop()
{
    stackElement* new_top = s_top;
    //update top
    s_top = s_top->link;
    // remove the top
    free(new_top);
}

int isEmpty()
{
    if(s_top == NULL)
    {
        return 1;
    }

    else
    {
        return 0;
    }
}


// hash function for non terminals
int get_hash_nonTerminal(const char* s) 
{
    int n = strlen(s);
    long long p = 31, m = 1157;
    long long hash = 0;
    long long p_pow = 1;
    for(int i = 0; i < n; i++) {
        hash = (hash + (s[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return abs(hash);
}

int get_hash_terminal(const char* s) 
{
    int n = strlen(s);
    long long p = 31, m = 1457;
    long long hash = 0;
    long long p_pow = 1;
    for(int i = 0; i < n; i++) {
        hash = (hash + (s[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return abs(hash);
}

void getNextToken()
{
    generateToken = 1;  
}

//function for displaying error while parsing
void display_error()
{
   if(global_token.hasError)
   {
        printf("Lexical error at line no. %d \n",global_token.line_no);
   }

   else
   {
        printf("Parsing error at line no. %d", global_token.line_no);
   }
    
}

// function for storing grammar rules in the form of linked list
void makeGrammar(FILE* fp)
{
    char buff[1024];
    int counter = 0;
    char delim[] = {", \n"};
    
    while(fgets(buff, 1024, fp))
    {
        char * token = strtok(buff, delim);
        struct node* nonTerminal = (struct node*) malloc(sizeof(struct node));
        nonTerminal->isTerminal = 0;
        nonTerminal->backward_link = NULL;
        //nonTerminal->value = token;
        strcpy(nonTerminal->value,token);
        grammar[counter] = nonTerminal;
        struct node* curr_token = nonTerminal;
        //printf("%s->",token);

        while((token = strtok(NULL, delim)) != NULL)
        {
            struct node* temp = (struct node*) malloc(sizeof(struct node));
            if(islower(token[0])) 
                temp->isTerminal = 0;  
            else
                temp->isTerminal = 1;

            // temp->value = token;
            strcpy(temp->value,token);
            curr_token->forward_link = temp;
            temp->backward_link = curr_token;
            curr_token = temp;
            //printf("%s,",token);
        }
        curr_token->forward_link = NULL;
        counter++;
        //printf("\n");
    }
}

void createParseTable()
{
    for(int i = 0; i < 128; i++)
    {
        int j = 0;
        for(int j = 0; strcmp(firstAndFollow[i][j],"end") != 0; j++)
        {
            parseTable[get_hash_nonTerminal(grammar[i]->value)][get_hash_terminal(firstAndFollow[i][j])] = grammar[i];
        }
    }   
}


void runPDA()
{
    if(s_top == NULL)
        display_error();

    else if(global_token.hasError)
    {
        display_error();
    }
    else
    {
        if(s_top->isTerminal)
        {
            if(!strcmp(s_top->value,token_strings[global_token.tk_name]))
            {
                s_pop();
                if(currExpand->nextSibling != NULL)
                    currExpand = currExpand->nextSibling;
                else
                {
                    while(currExpand->prevSibling != NULL)
                        currExpand = currExpand->prevSibling;

                    while(currExpand->parent->nextSibling == NULL && currExpand->parent != root)
                    {
                        currExpand = currExpand->parent;
                        while(currExpand->prevSibling != NULL)
                            currExpand = currExpand->prevSibling;
                    }

                    if(currExpand->parent != root)
                    {
                        currExpand = currExpand->parent->nextSibling;
                    }
                }
            }
            else
                display_error();
        }
        else
        {
            if(parseTable[get_hash_nonTerminal(s_top->value)][get_hash_terminal(token_strings[global_token.tk_name])] != NULL)
            {
                s_pop();
                struct node* curr;
                curr = parseTable[get_hash_nonTerminal(s_top->value)][get_hash_terminal(token_strings[global_token.tk_name])];
                
                //tree generation
                addChild(currExpand,curr->forward_link);
                currExpand = currExpand->children;

                while(curr->forward_link != NULL)
                {
                    curr = curr->forward_link;
                }
                while(curr->backward_link != NULL)
                {
                    stackElement* stackNode = malloc(sizeof(stackElement));
                    stackNode->isTerminal = curr->isTerminal;
                    strcpy(stackNode->value,curr->value);
                    s_push(stackNode);
                    curr = curr->backward_link;
                } 
            }
            else
            {
                display_error();
            }
        }
    }
}


int main()
{
    FILE* fp = fopen("grammar.txt","r");
    makeGrammar(fp);
    return 0;
}

