#include<stdio.h>
#include<string.h>
//integer = 8
//real,declare = 0
//of,array,use = 3
//start = 18
//end = 11
//module = 16
//driver,case = 22
//program = 10
// get_value,parameters,takes = 26
//print = 17
//with = 24
//input = 20
// returns,break,and = 7
// for,while = 27
//in = 5
//switch.true = 28
//default = 21
// or = 15
//false = 13

void swap(int* xp, int* yp)
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
 

int main(){
    char arr[30][11]={"integer","real","of","array","start","end","declare","module","driver","program","get_value","print","use","with","parameters","takes","input","returns","for","in","switch","case","break","default","while","AND","OR","true","false"};
    int hash[30];
    for(int i=0;i<30;i++){
        int size = strlen(arr[i]);
        int sum = 0;
        for(int j=0;j<size;j++){
            sum+= arr[i][j];
        }
        //printf("%s,%d \n",arr[i],sum%43);
        hash[i] = sum%97;
        if(!hash[i]){
            printf("* %s *",arr[i]);
        }
    }
    bubbleSort(hash,30);
    for(int i=0;i<30;i++)
        printf("%d ",hash[i]);
    return 0;
}


//97