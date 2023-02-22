#include"lexer.h"

char* tokenName[57] = {"INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER",
"PROGRAM","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TAKES","INPUT","RETURNS","FOR","IN","SWITCH",
"CASE","BREAK","DEFAULT","WHILE","AND","OR","TRUE","FALSE","ID","NUM","RNUM","PLUS","MINUS","MUL","DIV",
"LT","LE","GE","GT","EQ","NE","DEF","ENDDEF","DRIVERDEF","DRIVERENDDEF","COLON","RANGEOP","SEMICOL",
"COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","COMMENTMARK"};

   // integer : 71 
// real : 32
// of : 19
// array : 58
// start : 73
// end : 20
// declare : 41
// module : 64
// driver : 70
// program : 81
// get_value : 83
// print : 72
// use : 42
// with : 56
// parameters : 9
// takes : 51
// input : 75
// returns : 11
// for : 36
// in : 21
// switch : 76
// case : 24
// break : 32
// default : 62
// while : 52
// AND : 17
// OR : 64
// true : 60
// false : 38
//  : 0

int get_hash(char* str){
    int size = strlen(str);
    int sum = 0;
    for(int i=0;i<size;i++){
        sum+= str[i];
    }
    return sum%97;
}

void initHashTable(){
    char arr[30][11]={"integer","real","boolean","of","array","start","end","declare","module","driver","program","get_value","print","use","with","parameters","takes","input","returns","for","in","switch","case","break","default","while","AND","OR","true","false"};
    enum TOKEN tokens[30] = {INTEGER,REAL,BOOLEAN,OF,ARRAY,START,END,DECLARE,MODULE,DRIVER,
                            PROGRAM,GET_VALUE,PRINT,USE,WITH,PARAMETERS,TAKES,INPUT,RETURNS,FOR,IN,SWITCH,
                            CASE,BREAK,DEFAULT,WHILE,AND,OR,TRUE,FALSE};    
    int flag1=0,flag2=0;
    for(int i=0;i<30;i++){
        if(get_hash(arr[i]) == 32 && flag1 == 0){
            flag1=1;
            strcpy(hashTable[33].str,arr[i]);
            hashTable[33].tk = tokens[i];
        }
        else if(get_hash(arr[i]) == 64 && flag2 == 0){
            flag2=1;
            strcpy(hashTable[65].str,arr[i]);
            hashTable[65].tk = tokens[i];
        }
        else{
            strcpy(hashTable[get_hash(arr[i])].str,arr[i]);
            hashTable[get_hash(arr[i])].tk = tokens[i];
        }
    }
}

void check_begin(){
    if(flag==1 && begin >= buffer_size){
        begin=begin-buffer_size;
        flag = 3;
    }
    else if(flag==2 && begin >= buffer_size){
        begin = begin-buffer_size;
        flag = 0;
    }
}

void check_forward(){
    if(flag==1 && forward<0){
        forward=buffer_size-1;
        flag =0;
    }
    else if(flag==2 && forward<0){
        forward=buffer_size-1;
        flag =3;
    }
}

int check_keyword(char* lexeme){
    int hash_value = get_hash(lexeme);
    char string[21];
    if(hash_value == 32 || hash_value == 64){
        strcpy(string,hashTable[hash_value].str);
        if(!strcmp(string,lexeme)){
            tokenise(hashTable[hash_value].tk);
            return 1;
        }
        else{
            strcpy(string,hashTable[hash_value+1].str);
            if(!strcmp(string,lexeme)){
                tokenise(hashTable[hash_value+1].tk);
                return 1;
            }
        }
    }
    else{
        strcpy(string,hashTable[hash_value].str);
        if(!strcmp(string,lexeme)){
            tokenise(hashTable[hash_value].tk);
            return 1;
        }
    }
    return 0;     
}

void getnextblock(FILE* fp,char * buff){
        fread(buff,buffer_size,1,fp);
}

char getnextchar(FILE *fp,char *buff1,char *buff2){
    switch(flag){
        case 0:
        if(forward == buffer_size){
            getnextblock(fp,buff2);
            forward = 0;
            flag = 1;
            check_begin();
            return buff2[forward++]; 
        }
        return buff1[forward++];
        break;
        case 1:
        if(forward == buffer_size){     // This case will only be possible for comments, hence begin pointr doesnt matter only forward pointer matters
            getnextblock(fp,buff1);
            forward = 0;
            flag = 0;
            return buff1[forward++]; 
        }
        return buff2[forward++];
        break;
        case 2:
        if(forward == buffer_size){     // This case will only be possible for comments, hence begin pointr doesnt matter only forward pointer matters
            getnextblock(fp,buff2);
            forward = 0;
            flag = 3;
            check_begin();
            return buff2[forward++]; 
        }
        return buff1[forward++];
        break;
        case 3:
        if(forward == buffer_size){
            getnextblock(fp,buff1);
            forward = 0;
            flag = 2;
            check_begin();
            return buff1[forward++]; 
        }
      
        return buff2[forward++];
        break;
    }
}

void error_handle(){
    //likha hai ye
    printf("\n inside error handle, line number is %d, state is % d, breaking program here\n",current_line_no,state);
    exit(0);
}

void copy_lexeme(char * str){
    // mark ending by '\0'
    int forward2=forward-1;
    int flag2=flag;
    if(flag2==1 && forward2<0){
        forward2=buffer_size-1;
        flag2=0;
    }
    else if(flag2==2 && forward2<0){
        forward2=buffer_size-1;
        flag2=3;
    }
    if(flag2==0){
        int i=0;
        for(i=begin;i<forward2;i++){
            str[i-begin]=buff1[i];
        }
        str[i-begin] = '\0';

    }
    else if(flag2==1){
        int i=0;
        for(i=begin;i<buffer_size;i++){
            str[i-begin]=buff1[i];
        }
        for(i=0;i<forward2;i++){
            str[buffer_size-begin+i]=buff2[i];
        }
        str[buffer_size-begin+forward2]='\0';
    }
    else if(flag2==2){
        int i=0;
        for(i=begin;i<buffer_size;i++){
            str[i-begin]=buff2[i];
        }
        for(i=0;i<forward2;i++){
            str[buffer_size-begin+i]=buff1[i];
        }
        str[buffer_size-begin+forward2]='\0';
    }
    else{
        int i=0;
        for(i=begin;i<forward2;i++){
            str[i-begin]=buff2[i];
        }
        str[i-begin] = '\0';
    }
}
//abc+-

void tokenise(enum TOKEN tk_name){
     if(tk_name == ID){
        copy_lexeme(global_token.tk_data.lexeme);
        printf("\n Lexeme is '%s'",global_token.tk_data.lexeme);
     }
     else if(tk_name == NUM){
        char str[11];
        copy_lexeme(str);
        global_token.tk_data.val = atoi(str);
        printf("\n Value of integer is %d",global_token.tk_data.val);
     }
     else if(tk_name == RNUM){
        char str[21];
        copy_lexeme(str);
        global_token.tk_data.realVal = atof(str);
        printf("\n Value of real is %f",global_token.tk_data.realVal);
     }

     global_token.line_no = current_line_no;
     global_token.tk_name = tk_name;
     forward--;
     check_forward();
     state = 0;
     begin = forward;
     printf("\n token is %s, line no is %d \n\n",tokenName[global_token.tk_name],global_token.line_no);
     if(flag==1)flag=3;
     else if(flag==2)flag=0;
}

void dfa(char input){
    switch(state){
        case 0:
        if(isalpha(input) || input == '_')
            state = 1;
        else if(isdigit(input))
            state = 2;
        else if(input == '+')
            state = 8;
        else if(input == '-')
            state = 9;
        else if(input == '*')
            state = 10;
        else if(input == '/')
            state = 14;
        else if(input == '<')
            state = 15;
        else if(input == '>')
            state = 19;
        else if(input == '=')
            state = 23;
        else if(input == '!')
            state = 25;
        else if(input == ':')
            state = 27;
        else if(input ==',')
            state = 29;
        else if(input == '.')
            state = 30;
        else if(input == ';')
            state = 32;
        else if(input == '[')
            state = 33;
        else if(input == ']')
            state = 34;
        else if(input == '(')
            state = 35;
        else if(input == ')')
            state = 36;
        else if(input == ' ' || input == '\t'){
               state = 37;
               if(input == ' ')begin++;
               else begin+=4;
               check_begin();
        }    
        else if(input == '\n')
            state = 38;
        else
            error_handle();
        break;

        case 1:
        if(!isdigit(input) && !isalpha(input) && input!='_'){
             // check if it is a keyword, call check_keyword
            char str[21];
            copy_lexeme(str);
            if(!check_keyword(str)){
                tokenise(ID);
            }
        }
        break;

        case 2:
        if(input == '.')
            state = 3;
        else if(!isdigit(input))
            tokenise(NUM);
        break;

        case 3:
        if(isdigit(input))
            state = 4;
        else if(input == '.'){
            forward--;
            check_forward();
            tokenise(NUM);
        }
        else
            error_handle();
        break;

        case 4:
        if(input == 'E' || input == 'e')
            state = 5;
        else if(!isdigit(input)){
            tokenise(RNUM);
        }
        break;

        case 5:
        if(input == '+' || input == '-')
            state = 6;
        else if(isdigit(input))
            state = 7;
        else
            error_handle();
        break;

        case 6:
        if(isdigit(input))
            state = 7;
        else
            error_handle();
        break;

        case 7:
        if(!isdigit(input)){
            // iski value store karne me dikkat hai
            tokenise(RNUM);
        }
        break;

        case 8:
        tokenise(PLUS);
        break;

        case 9 : 
        tokenise(MINUS);    
        break;

        case 10:
        if(input=='*')
            state = 11;
        else 
            tokenise(MUL);
        break;

        case 11:
        if(input == '*')
            state = 12;
        else if(input == '\n'){
            current_line_no++;
        }
        break;

        case 12:
        if(input == '*')
            state = 13;
        else if(input == '\n'){
            state = 11;
            current_line_no++;
        }
        else
            state = 11;
        break;

        case 13:
        tokenise(COMMENTMARK);
        break;

        case 14:
        tokenise(DIV);
        break;

        case 15:
        if(input == '=')
            state = 18;
        else if(input == '<')
            state = 16;
        else
            tokenise(LT);
        break;

        case 16:
        if(input == '<')
            state = 17;
        else
            tokenise(DEF);
        break;

        case 17:
        tokenise(DRIVERDEF);
        break;

        case 18:
        tokenise(LE);
        break;
        
        case 19:
        if(input == '=')
            state = 20;
        else if(input == '>')
            state = 21;
        else
            tokenise(GT);
        break;
        
        case 20:
        tokenise(GE);
        break;

        case 21:
        if(input=='>')
            state = 22;
        else
            tokenise(ENDDEF);
        break;

        case 22:
        tokenise(DRIVERENDDEF);
        break;

        case 23:
        if(input=='=')
            state=24;
        else
            error_handle();
        break;

        case 24:
        tokenise(EQ);
        break;

        case 25:
        if(input == '=')
            state = 26;
        else
            error_handle();
        break;
        
        case 26:
        tokenise(NE);
        break;
        
        case 27:
        if(input == '='){
            state = 28;
        } 
        else{
            tokenise(COLON);
        }
        break;
        
        case 28:
        tokenise(ASSIGNOP);
        break;
        
        case 29:
        tokenise(COMMA);
        break;

        case 30:
        if(input == '.'){
            state = 31;
        }
        else{
            error_handle();
        }
        break;

        case 31:
        tokenise(RANGEOP);
        break;

        case 32:
        tokenise(SEMICOL);
        break;
        
        case 33:
        tokenise(SQBO);
        break;
        
        case 34:
        tokenise(SQBC);
        break;

        case 35:
        tokenise(BO);
        break;

        case 36:
        tokenise(BC);
        break;

        case 37:
        if(!(input==' ') && !(input=='\t')){
            state = 0;
            forward--;
            check_forward();
        }
        else if(input==' ')begin++;
        else if(input=='\t')begin+=4;
        check_begin();
        break;

        case 38:
        current_line_no++;
        begin++;
        state = 0;
        forward--;
        check_begin();
        check_forward();
        break;
    }
}

void initFile(){
    FILE *fp = fopen("testcase.txt", "a");
    if(fp == NULL){
        printf("File not found");
        return;
    }
    fputc('\n',fp);
    fputc('$',fp);
    fclose(fp);
}


int main(){
    initHashTable();
    initFile(); // add $ character to end of file- temporary solution
    FILE *fp = fopen("testcase.txt", "r");
    if(fp == NULL){
        printf("File not found");
        return 0;
    }
    fread(buff1,buffer_size,1,fp);
    while(1){
            char input = getnextchar(fp,buff1,buff2);
            // printf("\n Input is %c, begin is %d, forward is %d, flag is %d, state is %d",input,begin,forward,flag,state);
            if(input == '$'){
                break;
            }
            dfa(input);
        
    }

    return 0;
}