#include <stdio.h>
#include <string.h>
// integer = 8
// real,declare = 0
// of,array,use = 3
// start = 18
// end = 11
// module = 16
// driver,case = 22
// program = 10
//  get_value,parameters,takes = 26
// print = 17
// with = 24
// input = 20
//  returns,break,and = 7
//  for,while = 27
// in = 5
// switch.true = 28
// default = 21
//  or = 15
// false = 13

char nonTerminals[125][23] = {"startprogram","program","moduleDeclarations","moduleDeclaration","otherModules",
"driverModule","module","ret","input_plist","moreList","output_plist","moreOutput",
"dataType","type","moduleDef","statements","statement","ioStmt","print_var","whichId2",
"boolvar","whichId","index","sign","aVar","var_id_num","newArithmeticExpr","startExpr",
"newA1","newA2","newTerm","newNextTerm","u1","after_u1","simpleStmt","asssignmentStmt",
"whichStmt","lvalueIDStmt","lvalueARRStmt","moduleReuseStmt","optional","idList",
"moreId","expression","arithmeticOrBooleanExpr","ab1","anyTerm","ab2","u","after_unary",
"arithmeticExpr","a1","term","a2","nextTerm","op1","op2","bop","relationalOp",
"declareStmt","conditionalStmt","caseStmt","post","value","default","iterativeStmt",
"range_for","range","INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER",
"PROGRAM","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TAKES","INPUT","RETURNS","FOR","IN","SWITCH",
"CASE","BREAK","DEFAULT","WHILE","AND","OR","TRUE","FALSE","ID","NUM","RNUM","PLUS","MINUS","MUL","DIV",
"LT","LE","GE","GT","EQ","NE","DEF","ENDDEF","DRIVERDEF","DRIVERENDDEF","COLON","RANGEOP","SEMICOL",
"COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","COMMENTMARK"};

int get_hash_terminal(const char* s) 
{
    int n = strlen(s);
    long long p = 31, m = 10e4;
    long long hash = 0;
    long long p_pow = 1;
    for(int i = 0; i < n; i++) {
        hash = (hash + (s[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return abs(hash);
}


void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubbleSort(int *arr, int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)

        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}

int main()
{
    int arr[125];
    // for (int i = 0; i < 124; i++)
    // {
    //     arr[i] = get_hash_terminal(nonTerminals[i]);
    // }

    printf("%d\n",get_hash_terminal("GET_VALUE"));
    for(int i=0;i<125;i++)
        arr[i] = get_hash_terminal(nonTerminals[i]);
         bubbleSort(arr,125);
    for(int i=0;i<125;i++)
        printf("%d ",arr[i]);
}

// 97