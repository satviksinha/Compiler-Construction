#include "parser.c"

void printParseTree(struct treeNode *node, FILE *outfile)
{
    if (node == NULL || !strcmp(node->value, "EPSILON"))
    {
        return;
    }

    if (node->children == NULL)
    {  
        if(!strcmp(node->value,"NUM"))
            fprintf(outfile,"Lexeme: ----, Line no: %d, Tk name: %s, ValueIfNumber: %d, Parent: %s, isLeaf: YES, NodeSymbol: ----\n",node->line_no,node->value,node->tk_data.val,node->parent->value);
        else if(!strcmp(node->value,"RNUM"))
            fprintf(outfile,"Lexeme: ----, Line no: %d, Tk name: %s, ValueIfNumber: %f, Parent: %s, isLeaf: YES, NodeSymbol: ----\n",node->line_no,node->value,node->tk_data.realVal,node->parent->value);
        else
            fprintf(outfile,"Lexeme: %s, Line no: %d, Tk_name: %s, ValueIfNumber: ----, Parent: %s, YES, NodeSymbol: ----\n",node->tk_data.lexeme,node->line_no,node->value,node->parent->value);
        //printf("%s\n", node->value);
        return;
    }
    //printf("%s\n",node->value);
    // print left child
    printParseTree(node->children, outfile);

    // print node
    if(node->parent!=NULL)
        fprintf(outfile,"Lexeme: ----, Line no: ----, Tk name: ----, ValueIfNumber: ----, Parent: %s, IsLeaf: NO, NodeSymbol: %s\n",node->parent->value,node->value);
    else
        fprintf(outfile,"Lexeme: ----, Line no: ----, Tk name: ----, ValueIfNumber: ----, Parent: ROOT, IsLeaf: NO, Node Symbol: %s\n",node->value);
    //fprintf(outfile,"%s\n", node->value);

    // printf("%s\n",node->value);

    // print other children
    struct treeNode *temp;
    temp = node->children->nextSibling;

    while (temp != NULL)
    {
        printParseTree(temp, outfile);
        temp = temp->nextSibling;
    }
    //printf("%s\n",node->value);
    return;
}

int main()
{

    // lexer initialisations

    buffer_size = 512;
    buff1 = (char *)malloc(buffer_size * sizeof(char));
    buff2 = (char *)malloc(buffer_size * sizeof(char));

    initHashTable();
    fp = fopen("testcase6.txt", "r");
    if (fp == NULL)
    {
        printf("File not found");
        return 0;
    }

    // parser initialisations

    // grammar generate
    FILE *fp1 = fopen("grammar.txt", "r");
    makeGrammar(fp1);
    // for(int i=0;i<NUM_RULES;i++)
    //     printf("%d,%s\n",i,grammar[i]->forward_link->value);
    fclose(fp1);
    memset(isEpsilon, 0, sizeof(isEpsilon));

    // create first
    for (int i = 0; i < TOTAL_TERM; i++)
    {
        char *non_terminal = nonTerminals[i];
        createfirst(non_terminal); // calculate first of lhs
        if (ntFirst[get_hash(non_terminal)][strlen(ntFirst[get_hash(non_terminal)]) - 1] == ',')
            ntFirst[get_hash(non_terminal)][strlen(ntFirst[get_hash(non_terminal)]) - 1] = '\0';
    }
    isEpsilon[get_hash("EPSILON")] = 1;
    // printf("%s\n",ntFirst[get_hash("op1")]);

    //  for (int i = 0; i < 126; i++)
    //       printf("%s:%s\n", nonTerminals[i], ntFirst[get_hash(nonTerminals[i])]);

    // create follow
    for (int i = 0; i < NON_TERM; i++)
    {
        char *non_terminal = nonTerminals[i];
        // printf("%s\n", non_terminal);
        createFollow(non_terminal); // calculate first of lhs
        removeDuplicates(ntFollow[get_hash(non_terminal)]);
    }

    // for(int i=0;i<NON_TERM;i++){
    //     printf("%s:%s\n",nonTerminals[i],ntFollow[get_hash(nonTerminals[i])]);
    // }

    // compute first and follow
    computeFirstAndFollow();
    //  for (int i = 0; i < 126; i++)
    //       printf("%s:%s\n", nonTerminals[i], ntFirst[get_hash(nonTerminals[i])]);
    // for (int i = 0; i < 129; i++)
    // {
    //     printf("RULE:%d  ", i+1);
    //     for (int j = 0; firstAndFollow[i][j] != NULL; j++){
    //         printf("%s ",firstAndFollow[i][j]);
    //     }
    //     printf("\n");
    // }

    // create parse table
    createParseTable();
    // for(int i=0;i<1519;i++){
    //     for(int j=0;j<1519;j++){
    //         if(parseTable[i][j]!=NULL)
    //             printf("%d,%d:%s\n",i,j,parseTable[i][j]->value);
    //     }
    // }

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
    driverFlag = 0;
    while (!driverFlag)
    {
        // global_token.hasError = 0;
        getNextToken();
        // while (generateToken)
        // {
        //     char input = getnextchar(fp, buff1, buff2);
        //     dfa(input);
        //     if (input == EOF)
        //     {
        //         //   printf("input over\n");
        //         flag = 1;
        //         break;
        //     }
        // }
        // if (!driverFlag && global_token.tk_name != COMMENTMARK && global_token.hasError==0)
        runPDA();
    }
    while (isEpsilon[get_hash(s_top->value)])
        s_pop();

    if (!strcmp(s_top->value, "DOLLAR") && !errorToken)
        printf("Parsing successful");
    else if (errorToken)
        printf("Syntax Error\n");
    else
    {
        printf("Input finish but stack not empty");
        while (s_top != NULL)
        {
            printf("%s\n", s_top->value);
            s_pop();
        }
    }

    printf("\n%s\n",root->value);
    FILE *outfile = fopen("outfile.txt", "w");
    fprintf(outfile, "PARSE TREE:\n");
    printParseTree(root, outfile);
    fclose(fp);

    return 0;
}