#include "lexer.c"
#include "parser.c"
#include <time.h>

void removeComments(char *name){
    FILE *fp = fopen(name, "r");
    if (fp == NULL)
    {
        printf("File not found");
        return;
    }
    int flag = 0;
    while (!feof(fp))
    {
        char c = fgetc(fp);
        int flag2 = 0;
        if (c == '*' && flag == 0)
        {
            char d = fgetc(fp);
            if (d == '*')
            {
                flag = 1;
            }
            fseek(fp, -1, SEEK_CUR);
        }
        else if (c == '*' && flag == 1)
        {
            char d = fgetc(fp);
            if (d == '*')
            {
                flag = 0;
                printf("  ");
                flag2 = 1;
            }
            else
                fseek(fp, -1, SEEK_CUR);
        }
        if (!flag2)
        {
            if (flag == 0)
                printf("%c", c);
            else
                printf(" ");
        }
    }
    fclose(fp);
}

void print_token(){
    if (global_token.hasError == 1)
    {
        printf("\n %d \t \t %s \t \t LEXICAL ERROR \n", global_token.line_no, global_token.tk_data.lexeme);
    }
    else
    {
        if (global_token.tk_name == ID)
        {
            printf("%d \t \t %s \t \t %s \n", global_token.line_no, global_token.tk_data.lexeme, token_strings[global_token.tk_name]);
        }
        else if (global_token.tk_name == NUM)
        {
            printf("%d \t \t %d \t \t %s \n", global_token.line_no, global_token.tk_data.val, token_strings[global_token.tk_name]);
        }
        else if (global_token.tk_name == RNUM)
        {
            printf("%d \t \t %f \t \t %s \n", global_token.line_no, global_token.tk_data.realVal, token_strings[global_token.tk_name]);
        }
        else
        {
            printf("%d \t \t %s \t \t %s \n", global_token.line_no, global_token.tk_data.lexeme, token_strings[global_token.tk_name]);
        }
    }
}
void print_choices(){
    printf("\n Enter one option out of below: \n ***********");
    printf("\n 0. Exit");
    printf("\n 1. Print comment free code on terminal:");
    printf("\n 2. Print token list");
    printf("\n 3. Print parse tree on terminal:");
    printf("\n 4. Print total time taken by the stage 1 code: \n *********** \n");
}

void totalTime(char* name){


    // lexer initialisations

    initHashTable();
    FILE *fp=fopen(name,"r");
    if (fp == NULL)
    {
        printf("Source File not found");
        return;
    }

    // parser initialisations

    // grammar generate
    FILE *fp1 = fopen("grammar.txt", "r");
    makeGrammar(fp1);
    fclose(fp1);
    memset(isEpsilon, 0, sizeof(isEpsilon));

    // create first
    for (int i = 0; i < 126; i++)
    {
        char *non_terminal = nonTerminals[i];
        createfirst(non_terminal); // calculate first of lhs
        if (ntFirst[get_hash(non_terminal)][strlen(ntFirst[get_hash(non_terminal)]) - 1] == ',')
            ntFirst[get_hash(non_terminal)][strlen(ntFirst[get_hash(non_terminal)]) - 1] = '\0';
    }
    isEpsilon[get_hash("EPSILON")] = 1;
    
    for (int i = 0; i < 68; i++)
    {
        char *non_terminal = nonTerminals[i];
        // printf("%s\n", non_terminal);
        createFollow(non_terminal); // calculate first of lhs
        removeDuplicates(ntFollow[get_hash(non_terminal)]);
    }

    // compute first and follow
    computeFirstAndFollow();
   

    // create parse table
    createParseTable();
    

    // stack initialisations
    stackElement *element = malloc(sizeof(stackElement));
    element->isTerminal = 0;
    strcpy(element->value, "startprogram");
    s_push(element);

    // root element of tree creation
    struct treeNode *tree_node = malloc(sizeof(struct treeNode));
    tree_node->children = NULL;
    tree_node->isTerminal = 0;
    tree_node->parent = NULL;
    tree_node->nextSibling = NULL;
    tree_node->prevSibling = NULL;
    strcpy(tree_node->value, "startprogram");

    // setting root and currentExpand
    root = tree_node;
    currExpand = tree_node;

    getnextblock(fp, buff1);
    int flag = 0;
    while (!flag)
    {
        // global_token.hasError = 0;
        getNextToken();
        while (generateToken)
        {
            char input = getnextchar(fp, buff1, buff2);
            dfa(input);
            if (input == EOF)
            {
                flag = 1;
                break;
            }
        }
        if (!flag && global_token.tk_name != COMMENTMARK && global_token.hasError==0)
            runPDA();
    }
    while (isEpsilon[get_hash(s_top->value)])
        s_pop();

    if (!strcmp(s_top->value, "DOLLAR") && !errorToken)
        printf("Parsing successful");
    else
    {
        printf("Input finish but stack not empty");
        // while(s_top!=NULL){
        //     printf("%s\n",s_top->value);
        //     s_pop();
        // }
    }
    fclose(fp);
    return;
}

void printParseTree(struct treeNode *node,FILE* outfile)
{
    if (node == NULL || !strcmp(node->value,"EPSILON"))
    {
        return;
    }
    if (node->children == NULL)
    {
        // print node
        fprintf(outfile,"%s\n", node->value);
        return;
    }

    // print left child
    printParseTree(node->children,outfile);

    // print node
    fprintf(outfile,"%s\n", node->value);

    // print other children
    struct treeNode *temp;
    temp = node->children->nextSibling;

    while (temp != NULL)
    {
        printParseTree(temp,outfile);
        temp = temp->nextSibling;
    }
    return;
}

void runParser(FILE* parsetree, FILE* fp){
        
    // lexer initialisations

    initHashTable();
    if (fp == NULL)
    {
        printf("Source code file path incorrect \n");
        return;
    }

    // parser initialisations

    // grammar generate
    FILE *fp1 = fopen("grammar.txt", "r");
    makeGrammar(fp1);
    fclose(fp1);
    memset(isEpsilon, 0, sizeof(isEpsilon));

    // create first
    for (int i = 0; i < 126; i++)
    {
        char *non_terminal = nonTerminals[i];
        createfirst(non_terminal); // calculate first of lhs
        if (ntFirst[get_hash(non_terminal)][strlen(ntFirst[get_hash(non_terminal)]) - 1] == ',')
            ntFirst[get_hash(non_terminal)][strlen(ntFirst[get_hash(non_terminal)]) - 1] = '\0';
    }
    isEpsilon[get_hash("EPSILON")] = 1;
    
    for (int i = 0; i < 68; i++)
    {
        char *non_terminal = nonTerminals[i];
        // printf("%s\n", non_terminal);
        createFollow(non_terminal); // calculate first of lhs
        removeDuplicates(ntFollow[get_hash(non_terminal)]);
    }

    // compute first and follow
    computeFirstAndFollow();
   

    // create parse table
    createParseTable();
    

    // stack initialisations
    stackElement *element = malloc(sizeof(stackElement));
    element->isTerminal = 0;
    strcpy(element->value, "startprogram");
    s_push(element);

    // root element of tree creation
    struct treeNode *tree_node = malloc(sizeof(struct treeNode));
    tree_node->children = NULL;
    tree_node->isTerminal = 0;
    tree_node->parent = NULL;
    tree_node->nextSibling = NULL;
    tree_node->prevSibling = NULL;
    strcpy(tree_node->value, "startprogram");

    // setting root and currentExpand
    root = tree_node;
    currExpand = tree_node;

    getnextblock(fp, buff1);
    int flag = 0;
    while (!flag)
    {
        // global_token.hasError = 0;
        getNextToken();
        while (generateToken)
        {
            char input = getnextchar(fp, buff1, buff2);
            dfa(input);
            if (input == EOF)
            {
                flag = 1;
                break;
            }
        }
        if (!flag && global_token.tk_name != COMMENTMARK && global_token.hasError==0)
            runPDA();
    }
    while (isEpsilon[get_hash(s_top->value)])
        s_pop();

    if (!strcmp(s_top->value, "DOLLAR"))
        printf("Parsing successful");
    else
    {
        printf("Input finish but stack not empty");
        // while(s_top!=NULL){
        //     printf("%s\n",s_top->value);
        //     s_pop();
        // }
    }
    fprintf(parsetree,"PARSE TREE:\n");
    printParseTree(root,parsetree);
    return;
}

int main(int argc, char *argv[]){

    printf("\nImplementation Details\n(a) FIRST and FOLLOW set automated\n(b)Both lexical and syntax analysis modules implemented and work properly\n(c) modules pass all test cases\n(d) parse tree printed into file\n");
    int option;

    // buffer_size=atoi(argv[3]);
    // buff1 = (char*)malloc(buffer_size * sizeof(char));
    // buff2 = (char*)malloc(buffer_size * sizeof(char));

    print_choices();
    scanf("%d", &option);

    while (1)
    {
        
        switch (option)
        {
        case 0:
        {
            printf("\n Exiting succesfully \n");
            exit(0);
        }
        break;

        case 1:
        {
            // remove comments
            // printf("File name is %s \n",argv[1]);
            // FILE *input_file = fopen(argv[1], "r");
            removeComments(argv[1]);
        }
        break;

        case 2:
        {
            initHashTable();
            FILE *source_code = fopen(argv[1], "r");
            if (source_code == NULL)
            {
                printf("\n Source code file not found");
                exit(0);
            }
            printf("Line number  \t \t Lexeme \t \t Token Name: \n");
            getnextblock(source_code, buff1);
            int flag = 0;
            while (!flag)
            {
                // global_token.hasError = 0;
                getNextToken();
                while (generateToken)
                {
                    char input = getnextchar(source_code, buff1, buff2);
                    dfa(input);
                    if (input == EOF)
                    {
                        //   printf("input over\n");
                        flag = 1;
                        break;
                    }
                }
                print_token();
            }
        }
        break;

        case 3:
        {
            FILE* parseTreeFile = fopen(argv[2],"w");
            FILE* fp = fopen(argv[1],"r");
            
            runParser(parseTreeFile,fp);
            fclose(fp);
            fclose(parseTreeFile);
            printf("\n Parse Tree printed in txt file \n");

        }
        break;

        case 4:
        {
            clock_t start_time, end_time;
            double total_CPU_time, total_CPU_time_in_seconds;
            start_time = clock();

            // invoke your lexer and parser here
             totalTime(argv[1]);

            end_time = clock();
            total_CPU_time = (double)(end_time - start_time);
            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
            // Print both total_CPU_time and total_CPU_time_in_seconds
            printf("Total CPU Time is: %f, totoal cpu time in seconds is %f \n",total_CPU_time,total_CPU_time_in_seconds);
            
        }
        break;
        }

        print_choices();
        scanf("%d", &option);
    }
}