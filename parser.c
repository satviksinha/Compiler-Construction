# include "parser.h"


// hash function for non terminals
int get_hash(const char* s) {
    int n = strlen(s);
    long long p = 31, m = 1e9 + 7;
    long long hash = 0;
    long long p_pow = 1;
    for(int i = 0; i < n; i++) {
        hash = (hash + (s[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash;
}

// function for storing grammar rules in the form of linked list
void makeGrammar(FILE* fp)
{
    char buff[1024];
    int counter = 1;
    char delim[] = {", "};
    
    while(fgets(buff, 1024, fp))
    {
        char * token = strtok(buff, delim);
        struct node* nonTerminal;
        nonTerminal->isTerminal = 0;
        nonTerminal->value = token;
        grammar[counter] = nonTerminal;
        struct node* curr_token = nonTerminal;

        while((token = strtok(NULL, delim)) != NULL)
        {
            struct node* temp;
            if(islower(token[0])) 
                temp->isTerminal = 0;  
            else
                temp->isTerminal = 1;

            temp->value = token;
            curr_token->link = temp;
            curr_token = temp;
        }
        counter++;
    }
}

void createParseTable()
{
    for(int i = 0; i < 128; i++)
    {
        int j = 0;
        for(int j = 0; strcmp(firstAndFollow[i][j],"end") != 0; j++)
        {
            parseTable[get_hash(grammar[i]->value)][get_hash(firstAndFollow[i][j])] = grammar[i];
        }
    }   
}

int main()
{
    return 0;
}