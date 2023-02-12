#include<stdio.h>

// double assign_value(char * str){
//     int flag_2 = 0;
//     char before_E[21];
//     int i;
//     for(i=0;str[i]!='\0';i++){
//         if(str[i] == 'E' || str[i] == 'e'){
//             i++;
//             flag_2 = 1;
//             break;
//         }
//         before_E[i] = str[i];
//     }
//     before_E[i] = '\0';
//     double mantissa = atof(before_E);
//     if(!flag_2)
//         return mantissa;
//     else{
//         char exponent[21];
//         if(str[i] == '+'){
//             i++;
//             int j,k;
//             for(j=i,k=0;str[j]!='\0';j++,k++)
//                 exponent[k] = str[j];
//             exponent[k] = '\0';
//             return 
//         }
//     }
// }

int main(){
    char str[7] = "1e9";
    printf("%Lf",atof(str));
    return 0;
}