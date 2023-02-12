#include"lexer.h"

void getnextblock(FILE* fp,char * buff){
    fread(buff,512,1,fp);
}

char getnextchar(FILE *fp,char *buff1,char *buff2){
    switch(flag){
        case 0:
        if(forward == 512){
            getnextblock(fp,buff2);
            forward = 0;
            flag = 1;
            return buff2[forward++]; 
        }
        return buff1[forward++];
        break;
        case 1:
        return buff2[forward++];
        break;
        case 2:
        return buff1[forward++];
        break;
        case 3:
        if(forward == 512){
            getnextblock(fp,buff1);
            forward = 0;
            flag = 2;
            return buff1[forward++]; 
        }
        return buff2[forward++];
        break;
    }
}

void error_handle(){
    //likha hai ye
}

void copy_lexeme(char * str){
    // mark ending by '\0'
}

void tokenise(enum TOKEN tk_name){
     global_token.line_no = current_line_no;
     global_token.tk_name = tk_name;
     if(tk_name == ID)
        copy_lexeme(global_token.tk_data.lexeme);
     else if(tk_name == NUM){
        char str[11];
        copy_lexeme(str);
        global_token.tk_data.val = atoi(str);
     }
     else if(tk_name == RNUM){
        char str[21];
        copy_lexeme(str);
        //satark rahe for E
        global_token.tk_data.realVal = atof(str);
     }
     state = 0;
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
        else if(input == ' ' || input == '\t')
            state = 37;
        else if(input == '\n')
            state = 38;
        else
            error_handle();
        break;

        case 1:
        if(!isdigit(input) && !isalpha(input) && input!='_'){
             // check if it is a keyword, call check_keyword
            tokenise(ID);
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
        else if(input == '.')
            state = 39;
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
        if(!input==' ' && !input=='\t'){
            state = 0;
        }
        break;

        case 38:
        current_line_no++;
        state = 0;
        break;
    }
}

int main(){
    FILE* fp = fopen("testcase.txt","r");
    getnextblock(fp,buff1);
    return 0;
}