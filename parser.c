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
char ntFirst[99500][300];  // size to be changed later
char ntFollow[99500][300]; // size to be changed later

int isEpsilon[99500];
int cnt = 0;
int createfirst(char *term)
{ // returns 0 if first contains epsilon,otherwise returns 1
  // printf("%s",term);

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
                                                       // printf("%s\n",ntFirst[get_hash(term)]);
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

void bubbleSort(char *arr[500], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (strcmp(arr[j], arr[j + 1]) > 0)
            {
                char *temp;
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
}

void removeDuplicates(char *str)
{
    char *arr[500];
    char *token = strtok(str, ",");
    int i = 0;
    while (token != NULL)
    {
        arr[i] = token;
        i++;
        token = strtok(NULL, ",");
    }
    bubbleSort(arr, i);
    int n = i;
    char str1[2000] = {};
    for (i = 0; i < n; i++)
    {
        if (i != n - 1 && strcmp(arr[i], arr[i + 1]))
        {
            if (strlen(str1))
            {
                strcat(str1, ",");
            }
            strcat(str1, arr[i]);
        }
        if (i == n - 1)
        {
            if (strlen(str1))
            {
                strcat(str1, ",");
            }
            strcat(str1, arr[i]);
        }
    }
    // printf("%s \n",str1);
    strcpy(str, str1);
}

void createFollow(char *non_terminal)
{
    if (strlen(ntFollow[get_hash(non_terminal)]))
        return;
    for (int i = 0; i < 129; i++)
    {
        struct node *temp = grammar[i]->link;
        while (temp != NULL && strcmp(temp->value, non_terminal))
        {
            temp = temp->link;
        }

        if (temp != NULL)
        {
            temp = temp->link;
            // printf("%s",temp->value);
            while (temp != NULL && isEpsilon[get_hash(temp->value)])
            {
                if (strlen(ntFollow[get_hash(non_terminal)]) && ntFollow[get_hash(non_terminal)][strlen(ntFollow[get_hash(non_terminal)]) - 1] != ',')
                    strcat(ntFollow[get_hash(non_terminal)], ",");
                strcat(ntFollow[get_hash(non_terminal)], ntFirst[get_hash(temp->value)]);
                temp = temp->link;
            }

            if (temp == NULL)
            {
                if (strcmp(grammar[i]->value, non_terminal))
                {
                    if (strlen(ntFollow[get_hash(non_terminal)]) && ntFollow[get_hash(non_terminal)][strlen(ntFollow[get_hash(non_terminal)]) - 1] != ',')
                        strcat(ntFollow[get_hash(non_terminal)], ",");
                    if (!strlen(ntFollow[get_hash(grammar[i]->value)]))
                        createFollow(grammar[i]->value);
                    strcat(ntFollow[get_hash(non_terminal)], ntFollow[get_hash(grammar[i]->value)]);
                }
            }
            else
            {
                // printf("%s",temp->value);
                if (strlen(ntFollow[get_hash(non_terminal)]) && ntFollow[get_hash(non_terminal)][strlen(ntFollow[get_hash(non_terminal)]) - 1] != ',')
                    strcat(ntFollow[get_hash(non_terminal)], ",");
                // printf("%s",ntFirst[get_hash(temp->value)]);
                strcat(ntFollow[get_hash(non_terminal)], ntFirst[get_hash(temp->value)]);
            }
        }
    }
}

void computeFirstAndFollow()
{
    for (int i = 0; i < 129; i++)
    {
        int j = 0;
        char *first;
        char *follow;
        char *token;
        struct node *temp = grammar[i]->link;
        while (temp != NULL && isEpsilon[get_hash(temp->value)])
        {
            if(strcmp(temp->value,"EPSILON")){
                first = ntFirst[get_hash(temp->value)];
                token = strtok(first, ",");
                firstAndFollow[i][j++] = token;
                while ((token = strtok(NULL, ",")) != NULL)
                {
                    firstAndFollow[i][j++] = token;
                }
            }
            temp = temp->link;
        }
        if (temp == NULL)
        {
            follow = ntFollow[get_hash(grammar[i]->value)];
            token = strtok(follow, ",");
            firstAndFollow[i][j++] = token;
            while ((token = strtok(NULL, ",")) != NULL)
            {
                firstAndFollow[i][j++] = token;
            }
        }
        else
        {
            first = ntFirst[get_hash(temp->value)];
            token = strtok(first, ",");
            firstAndFollow[i][j++] = token;
            while ((token = strtok(NULL, ",")) != NULL)
            {
                firstAndFollow[i][j++] = token;
            }
        }
    }
}

int main()
{
    FILE *fp = fopen("grammar.txt", "r");
    makeGrammar(fp);
    // memset(ntFirst,'-',1157*300);
    memset(isEpsilon, 0, sizeof(isEpsilon));
    for (int i = 0; i < 126; i++)
    {
        char *non_terminal = nonTerminals[i];
        createfirst(non_terminal); // calculate first of lhs
    }
    isEpsilon[get_hash("EPSILON")] = 1;
    // createfirst("arithmeticOrBooleanExpr");
    // for (int i = 0; i < 126; i++)
    //     printf("%s:%s\n", nonTerminals[i], ntFirst[get_hash(nonTerminals[i])]);
    for (int i = 0; i < 68; i++)
    {
        char *non_terminal = nonTerminals[i];
        // printf("%s\n", non_terminal);
        createFollow(non_terminal); // calculate first of lhs
        removeDuplicates(ntFollow[get_hash(non_terminal)]);
    }
    // createFollow("whichId");

    // //printf("%s\n\n",ntFollow[get_hash("whichId")]);
    // removeDuplicates(ntFollow[get_hash("whichId")]);
    // printf("%s",ntFollow[get_hash("whichId")]);

    // for (int i = 0; i < 68; i++)
    // {
    //     printf("%s:%s\n", nonTerminals[i], ntFollow[get_hash(nonTerminals[i])]);
    // }
    // printf("%s", ntFirst[get_hash("arithmeticOrBooleanExpr")]);
    computeFirstAndFollow();
    for (int i = 0; i < 129; i++)
    {
        printf("RULE:%d  ", i+1);
        for (int j = 0; firstAndFollow[i][j] != NULL; j++)
            printf("%s ", firstAndFollow[i][j]);
        printf("\n");
    }

    //printf("%s \n", ntFirst[get_hash("moduleDeclaration")]);

    return 0;
}