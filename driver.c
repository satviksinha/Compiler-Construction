#include "parser.c"
#include "lexer.c"

int main(){

    //lexer initialisations
    
    initHashTable();
    FILE *fp = fopen("testcase2.txt", "r");
    if(fp == NULL){
        printf("File not found");
        return 0;
    }

    //parser initialisations

    //grammar generate
    FILE *fp1 = fopen("grammar.txt", "r");
    makeGrammar(fp1);
    fclose(fp1);
    memset(isEpsilon, 0, sizeof(isEpsilon));

    //create first
    for (int i = 0; i < 126; i++)
    {
        char *non_terminal = nonTerminals[i];
        createfirst(non_terminal); // calculate first of lhs
        if(ntFirst[get_hash(non_terminal)][strlen(ntFirst[get_hash(non_terminal)])-1]==',')
            ntFirst[get_hash(non_terminal)][strlen(ntFirst[get_hash(non_terminal)])-1] = '\0';
    }
    isEpsilon[get_hash("EPSILON")] = 1;
   // printf("%s\n",ntFirst[get_hash("op1")]);

    //  for (int i = 0; i < 126; i++)
    //       printf("%s:%s\n", nonTerminals[i], ntFirst[get_hash(nonTerminals[i])]);

    //create follow
    for (int i = 0; i < 68; i++)
    {
        char *non_terminal = nonTerminals[i];
        // printf("%s\n", non_terminal);
        createFollow(non_terminal); // calculate first of lhs
        removeDuplicates(ntFollow[get_hash(non_terminal)]);
    }

    //compute first and follow
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

    //create parse table
    createParseTable();
    // for(int i=0;i<1519;i++){
    //     for(int j=0;j<1519;j++){
    //         if(parseTable[i][j]!=NULL)
    //             printf("%d,%d:%s\n",i,j,parseTable[i][j]->value);
    //     }
    // }

    //stack initialisations
    stackElement* element = malloc(sizeof(stackElement));
    element->isTerminal = 0;
    strcpy(element->value,"startprogram");
    s_push(element);

    //root element of tree creation
    struct treeNode* tree_node = malloc(sizeof(struct treeNode));
    tree_node->children = NULL;
    tree_node->isTerminal = 0;
    tree_node->parent = NULL;
    tree_node->nextSibling = NULL;
    tree_node->prevSibling = NULL;
    strcpy(tree_node->value,"startprogram");

    //setting root and currentExpand
    root = tree_node;
    currExpand = tree_node;

    getnextblock(fp,buff1);
    int flag=0;
    while(!flag)
    {
        // global_token.hasError = 0;
        getNextToken();
        while(generateToken){
            char input = getnextchar(fp,buff1,buff2);
            dfa(input);
            if(input==EOF){
            //   printf("input over\n");
              flag=1;
              break;
            }
        }
        if(!flag && global_token.tk_name!=COMMENTMARK)
            runPDA();
    }
    while(isEpsilon[get_hash(s_top->value)])
        s_pop();

    if(!strcmp(s_top->value,"DOLLAR"))
        printf("Parsing successful");
    else{
        printf("Input finish but stack not empty");
        // while(s_top!=NULL){
        //     printf("%s\n",s_top->value);
        //     s_pop();
        // }
    }

    fclose(fp);

    return 0;
}