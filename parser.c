# include "parser.h"


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
            curr_token->link = temp;
            curr_token = temp;
            //printf("%s,",token);
        }
        curr_token->link = NULL;
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

int main()
{
    FILE* fp = fopen("grammar.txt","r");
    makeGrammar(fp);
    return 0;
}