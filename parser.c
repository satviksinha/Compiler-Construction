#include "parserDef.h"
#include "lexerDef.h"
#include "treeDef.h"

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

//to be imported from tree.c
void addChild(struct treeNode* parent, struct node* child)
{
    struct treeNode* temp = malloc(sizeof(struct treeNode));
    strcpy(temp->value,child->value);
    temp->isTerminal = child->isTerminal;
    temp->parent = parent;
    temp->prevSibling = NULL;

    parent->children = temp;

    child = child->forward_link;
    
    while(child != NULL)
    {
        struct treeNode* temp2 = malloc(sizeof(struct treeNode));
        strcpy(temp2->value,child->value);
        temp2->isTerminal = child->isTerminal;
        temp2->parent = NULL;
        temp2->prevSibling = temp;
        temp->nextSibling = temp2;
        temp = temp2;
        child = child->forward_link;
    }

    temp->nextSibling = NULL;
}

//hash function for terminals and non-terminals
int get_hash(const char *s)
{
    int n = strlen(s);
    long long p = 31, m = 10e9+7;
    long long hash = 0;
    long long p_pow = 1;
    for (int i = 0; i < n; i++)
    {
        hash = (hash + (s[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return abs(hash)%1521;
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
        printf("Parsing error at line no. %d\n", global_token.line_no);
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
    for(int i = 0; i < 129; i++)
    {
        for(int j = 0; firstAndFollow[i][j] != NULL; j++)
        {
            // if(!strcmp(firstAndFollow[i][j],"ID"))
            //     printf("lol,%d,%d",get_hash(firstAndFollow[i][j]),get_hash("ID"));
            parseTable[get_hash(grammar[i]->value)][get_hash(firstAndFollow[i][j])] = grammar[i];
        }
    }
}


void runPDA()
{
    if(s_top == NULL)
        display_error();
    else if(global_token.hasError)
        display_error();
    else
    {
        // printf("%d",s_top->isTerminal);
        if(s_top->isTerminal)
        {
            if(!strcmp(s_top->value,token_strings[global_token.tk_name]))
            {
                printf("match,%s\n",s_top->value);
                s_pop();
                //tree-code

                // if(currExpand->nextSibling != NULL)
                //     currExpand = currExpand->nextSibling;
                // else
                // {
                //     while(currExpand->prevSibling != NULL)
                //         currExpand = currExpand->prevSibling;

                //     while(currExpand->parent->nextSibling == NULL && currExpand->parent != root)
                //     {
                //         currExpand = currExpand->parent;
                //         while(currExpand->prevSibling != NULL)
                //             currExpand = currExpand->prevSibling;
                //     }

                //     if(currExpand->parent != root)
                //     {
                //         currExpand = currExpand->parent->nextSibling;
                //     }
                // }
            }
            else{
                printf("not match");
                display_error();
            }
        }
        else
        {   
            //printf("%d,%d",get_hash(s_top->value),get_hash("ID"));
            if(parseTable[get_hash(s_top->value)][get_hash(token_strings[global_token.tk_name])] != NULL)
            {
                struct node* curr;
                //printf("Before curr");
                curr = parseTable[get_hash(s_top->value)][get_hash(token_strings[global_token.tk_name])];
                s_pop();
                //tree generation
                //printf("Before add child");
                // addChild(currExpand,curr->forward_link);
                //  printf("After add child");
                // currExpand = currExpand->children;
                //printf("After curr expand");
                while(curr->forward_link != NULL)
                {
                    curr = curr->forward_link;
                }
                while(curr->backward_link != NULL)
                {
                    //printf("inside while loop\n");
                    stackElement* stackNode = malloc(sizeof(stackElement));
                    stackNode->isTerminal = curr->isTerminal;
                    strcpy(stackNode->value,curr->value);
                    if(strcmp(stackNode->value,"EPSILON"))
                        s_push(stackNode);
                    //printf("%s,%d\n",s_top->value,s_top->isTerminal);
                    curr = curr->backward_link;
                }
                runPDA();
            }
            else
            {
                printf("lol");
                display_error();
            }
        }
    }
}


// int already_visited[128];
char ntFirst[1519][300];  // size to be changed later
char ntFollow[1519][300]; // size to be changed later

int isEpsilon[1519];
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
            struct node *temp = grammar[i]->forward_link; // LHS
            while (temp != NULL && createfirst(temp->value))
            {                                                                                                               // if temp is a terminal (except epsilon) or a nt (which doesnt derive epsilon)
                if (strlen(ntFirst[get_hash(term)]) && ntFirst[get_hash(term)][strlen(ntFirst[get_hash(term)]) - 1] != ',') // if ntFirst of that term isnt empty
                    strcat(ntFirst[get_hash(term)], ",");
                strcat(ntFirst[get_hash(term)], ntFirst[get_hash(temp->value)]);
                temp = temp->forward_link;
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
        struct node *temp = grammar[i]->forward_link;
        while (temp != NULL && strcmp(temp->value, non_terminal))
        {
            temp = temp->forward_link;
        }

        if (temp != NULL)
        {
            temp = temp->forward_link;
            // printf("%s",temp->value);
            while (temp != NULL && isEpsilon[get_hash(temp->value)])
            {
                if (strlen(ntFollow[get_hash(non_terminal)]) && ntFollow[get_hash(non_terminal)][strlen(ntFollow[get_hash(non_terminal)]) - 1] != ',')
                    strcat(ntFollow[get_hash(non_terminal)], ",");
                strcat(ntFollow[get_hash(non_terminal)], ntFirst[get_hash(temp->value)]);
                temp = temp->forward_link;
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
        struct node *temp = grammar[i]->forward_link;
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
            temp = temp->forward_link;
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
    // for (int i = 0; i < 129; i++)
    // {
    //     printf("RULE:%d  ", i+1);
    //     for (int j = 0; firstAndFollow[i][j] != NULL; j++)
    //         printf("%s ", firstAndFollow[i][j]);
    //     printf("\n");
    // }
    //printf("%s*",firstAndFollow[11][0]);
    createParseTable();
    // for(int i=0;i<1519;i++){
    //     for(int j=0;j<1519;j++){
    //         if(parseTable[i][j]!=NULL)
    //             printf("%d,%d:%s\n",i,j,parseTable[i][j]->value);
    //     }
    // }
    
    stackElement* element = malloc(sizeof(stackElement));
    element->isTerminal = 0;
    strcpy(element->value,"startprogram");
    s_push(element);
    // printf("%s\n",s_top->value);

    global_token.hasError = 0;
    global_token.line_no = 1;
    strcpy(global_token.tk_data.lexeme,"<<<");
    global_token.tk_name = DRIVERDEF;

    runPDA();

    global_token.hasError = 0;
    global_token.line_no = 1;
    strcpy(global_token.tk_data.lexeme,"driver");
    global_token.tk_name = DRIVER;

    runPDA();

    global_token.hasError = 0;
    global_token.line_no = 1;
    strcpy(global_token.tk_data.lexeme,"program");
    global_token.tk_name = PROGRAM ;

    runPDA();

     global_token.hasError = 0;
    global_token.line_no = 1;
    strcpy(global_token.tk_data.lexeme,">>>");
    global_token.tk_name = DRIVERENDDEF;

    runPDA();

    global_token.hasError = 0;
    global_token.line_no = 2;
    strcpy(global_token.tk_data.lexeme,"start");
    global_token.tk_name = START;

    runPDA();


    global_token.hasError = 0;
    global_token.line_no = 3;
    strcpy(global_token.tk_data.lexeme,"a");
    global_token.tk_name = ID;

    runPDA();

    global_token.hasError = 0;
    global_token.line_no = 3;
    strcpy(global_token.tk_data.lexeme,"=");
    global_token.tk_name = ASSIGNOP;

    runPDA();

    global_token.hasError = 0;
    global_token.line_no = 3;
    strcpy(global_token.tk_data.lexeme,"b");
    global_token.tk_name = ID;

    runPDA();

    global_token.tk_name = PLUS;
    global_token.hasError = 0;
    global_token.line_no = 3;
    strcpy(global_token.tk_data.lexeme,"+");

    runPDA();

    global_token.tk_name = ID;
    global_token.hasError = 0;
    global_token.line_no = 3;
    strcpy(global_token.tk_data.lexeme,"c");

    runPDA();

    global_token.tk_name = SEMICOL;
    global_token.hasError = 0;
    global_token.line_no = 3;
    strcpy(global_token.tk_data.lexeme,";");

    runPDA();

    
    global_token.hasError = 0;
    global_token.line_no = 6;
    strcpy(global_token.tk_data.lexeme,"end");
    global_token.tk_name = END;

    runPDA();
    
    return 0;
}

