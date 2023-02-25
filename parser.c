#include "parser.h"

// hash function for non terminals
int get_hash_nonTerminal(const char *s)
{
    int n = strlen(s);
    long long p = 31, m = 1157;
    long long hash = 0;
    long long p_pow = 1;
    for (int i = 0; i < n; i++)
    {
        hash = (hash + (s[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return abs(hash);
}

int get_hash_terminal(const char *s)
{
    int n = strlen(s);
    long long p = 31, m = 1457;
    long long hash = 0;
    long long p_pow = 1;
    for (int i = 0; i < n; i++)
    {
        hash = (hash + (s[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return abs(hash);
}

int get_hash(const char *s)
{
    int n = strlen(s);
    long long p = 31, m = 10e4;
    long long hash = 0;
    long long p_pow = 1;
    for (int i = 0; i < n; i++)
    {
        hash = (hash + (s[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return abs(hash);
}

// function for storing grammar rules in the form of linked list
void makeGrammar(FILE *fp)
{
    char buff[1024];
    int counter = 0;
    char delim[] = {",\n"};

    while (fgets(buff, 1024, fp))
    {
        char *token = strtok(buff, delim);
        struct node *nonTerminal = (struct node *)malloc(sizeof(struct node));
        nonTerminal->isTerminal = 0;
        // nonTerminal->value = token;
        strcpy(nonTerminal->value, token);
        grammar[counter] = nonTerminal;
        struct node *curr_token = nonTerminal;
        // printf("%s->",token);

        while ((token = strtok(NULL, delim)) != NULL)
        {
            struct node *temp = (struct node *)malloc(sizeof(struct node));
            if (islower(token[0]))
                temp->isTerminal = 0;
            else
                temp->isTerminal = 1;

            // temp->value = token;
            strcpy(temp->value, token);
            curr_token->link = temp;
            curr_token = temp;
            // printf("%s,",token);
        }
        curr_token->link = NULL;
        counter++;
        // printf("\n");
    }
}

// void createParseTable()
// {
//     for(int i = 0; i < 128; i++)
//     {
//         int j = 0;
//         for(int j = 0; strcmp(firstAndFollow[i][j],"end") != 0; j++)
//         {
//             parseTable[get_hash_nonTerminal(grammar[i]->value)][get_hash_terminal(firstAndFollow[i][j])] = grammar[i];
//         }
//     }
// }

// int already_visited[128];
char ntFirst[99500][300]; // size to be changed later
int isEpsilon[99500];
int cnt = 0;
int createfirst(char *term)
{ // returns 0 if first contains epsilon,otherwise returns 1
     //printf("%s",term);

    if (strlen(ntFirst[get_hash(term)]))
    {
        // printf("%s",term);
        return isEpsilon[get_hash(term)];
    }
    // printf("%d:%s \n", isupper(term[0]), term);
    if (isupper(term[0]))
    { // if term is a terminal
        if (strcmp(term, "EPSILON"))
        {                                              // and its not an epsilon
            if (!strlen(ntFirst[get_hash(term)]))      // if ntFirst of that term is empty
                strcat(ntFirst[get_hash(term)], term); // append term
             //printf("%s\n",ntFirst[get_hash(term)]);
            return 0;
        }
        else
        {
            isEpsilon[get_hash(term)] = 1; // if term is epsilon set isEpsilon 1 and return 1
            return 1;
        }
    }

    // if term is a non terminal,find first
    for (int i = 0; i < 129; i++)
    {
        if (!strcmp(grammar[i]->value, term))
        {                                         // matching nt found
            struct node *temp = grammar[i]->link; // LHS
            while (temp != NULL && createfirst(temp->value))
            {                                                                                                               // if temp is a terminal (except epsilon) or a nt (which doesnt derive epsilon)
                if (strlen(ntFirst[get_hash(term)]) && ntFirst[get_hash(term)][strlen(ntFirst[get_hash(term)]) - 1] != ',') // if ntFirst of that term isnt empty
                    strcat(ntFirst[get_hash(term)], ",");
                strcat(ntFirst[get_hash(term)], ntFirst[get_hash(temp->value)]);
                temp = temp->link;
            }
            if (temp == NULL)
                isEpsilon[get_hash(term)] = 1;
            else
            {
                if (strlen(ntFirst[get_hash(term)]) && ntFirst[get_hash(term)][strlen(ntFirst[get_hash(term)]) - 1] != ',') // if ntFirst of that term isnt empty
                    strcat(ntFirst[get_hash(term)], ",");
                // printf("%s\n",ntFirst[get_hash(temp->value)]);
                strcat(ntFirst[get_hash(term)], ntFirst[get_hash(temp->value)]);
            }
        }
    }
    return isEpsilon[get_hash(term)];
}

void computeFirstAndFollow()
{
    for (int i = 0; i < 67; i++)
    {
        char *non_terminal = nonTerminals[i];
        createfirst(non_terminal); // calculate first of lhs
    }
}

int main()
{
    FILE *fp = fopen("grammar.txt", "r");
    makeGrammar(fp);
    // memset(ntFirst,'-',1157*300);
    memset(isEpsilon, 0, sizeof(isEpsilon));
    for (int i = 0; i < 125; i++)
    {
        char *non_terminal = nonTerminals[i];
        createfirst(non_terminal); // calculate first of lhs
    }
    // createfirst("arithmeticOrBooleanExpr");
    for (int i = 0; i < 125; i++)
        printf("%s:%s\n", nonTerminals[i], ntFirst[get_hash(nonTerminals[i])]);
    // printf("%s", ntFirst[get_hash("arithmeticOrBooleanExpr")]);
    return 0;
}