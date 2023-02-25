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
            else if(!strcmp(token,"EPSILON"))
                temp->isTerminal = 0;
            else
                temp->isTerminal=1;

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
int done[128];

char temp_store[100];

void computeFirst(struct node* curr,char* str,int i){
    done[i] = 1;
    if(curr->isTerminal==1){
        if(strlen(str))strcat(str,",");
        strcat(str,curr->value);
        return ;
    }
    else{
        //go to curr in grammar, for all rules of curr populate first, then return.
        // printf("\nIN HERE 3\n");
        int seen = 0;
        for(int j=i+1;j<128;j++){
            struct node* temp = grammar[j];
            // printf("\nIN HERE\n");
            if(!strcmp(temp->value, curr->value)){
            seen=1;
            // printf("\nIN HERE 2\n");
            int flag = 0;
            while(!flag){
            if(done[j]!=1){
                // printf("\n in here 4");
                computeFirst(temp->link,firstAndFollow[j],j);
                if(strlen(str))strcat(str,",");
                strcat(str,firstAndFollow[j]);
                strcpy(temp_store,firstAndFollow[j]);
                printf("\n First and follow of rule no %d is %s \n",j,firstAndFollow[j]);
                char *token = strtok(temp_store, ",");
                int epsilon_found=0;
                while (token != NULL)
                { 

                    if(!strcmp(token,"EPSILON")){
                        // printf("FOUND EPISLION \n");
                        epsilon_found=1;
                        temp = temp->link;
                        if(temp->link!=NULL){
                            flag=0;
                        }
                        else{
                            flag=1; 
                        }
                        
                    }
                    token = strtok(NULL, ",");
                }
                if(!epsilon_found)flag=1;               
            
            }
            }
            }
            else{
                if(seen==1) return;
            }
        }
    }
}

void computeFollow(){

}
void computeFirstAndFollow(){
    for(int i =0;i<128;i++){
        struct node* temp = grammar[i];

        if(done[i]!=1){
            int flag = 0;
            while(!flag){ 
            computeFirst(temp->link,firstAndFollow[i],i);
            printf("HERE");
            printf("\n First and follow of rule no %d is %s \n",i,firstAndFollow[i]);
            strcpy(temp_store,firstAndFollow[i]);
            char *token = strtok(temp_store, ",");
            int epsilon_found=0;
            while (token != NULL)
            {
                if(!strcmp(token,"EPSILON")){
                    printf("\n found epsilon down\n");
                    epsilon_found=1;
                    temp = temp->link;
                    if(temp->link!=NULL){
                        flag=0;
                    }
                    else{
                        flag=1; 
                    }
                    
                }
                token = strtok(NULL, ",");
            }
            if(!epsilon_found)flag=1;
 
        }
        }
    }
}

int main()
{
    FILE* fp = fopen("grammar.txt","r");
    makeGrammar(fp);
    for(int i=0;i<128;i++){
    done[i]=0;
}
    computeFirstAndFollow();
    for(int i=0;i<128;i++){
        printf("%s \n",firstAndFollow[i]);

    }
    return 0;
}