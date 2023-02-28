#include "lexer.c"
#include "parser.c"
#include <time.h>

void removeComments(FILE *fp)
{
    if (fp == NULL)
    {
        printf("File not found");
        return 0;
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
}

void print_token()
{
    if (global_token.hasError)
    {
        printf("%d \t \t %s \t \t LEXICAL ERROR \n", global_token.line_no, global_token.tk_data.lexeme);
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

int main(int argc, char *argv[])
{
    int option;
    print_choices();
    scanf("%d", &option);

    while (option)
    {
        switch (option)
        {
        case 0:
        {   
            printf("Exiting succesfully");
            exit(0);
        }
        break;

        case 1:
        {
            // remove comments
            FILE *input_file = fopen(argv[0], "r");
            removeComments(input_file);
            fclose(input_file);
        }
        break;

        case 2:
        {
            initHashTable();
            FILE *source_code = fopen(argv[0], "r");
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
        }
        break;

            print_choices();
            scanf("%d",option);
            
        }
    }
    }