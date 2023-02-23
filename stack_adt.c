# include "stack_adt.h"

// function definitions

void push(stackElement* element)
{
    //creating back link
    element->link = top;
    
    // change top
    top  = element;
}

void pop()
{
    stackElement* new_top = top;
    //update top
    top = top->link;
    
    // remove the top
    free(new_top);
}

int isEmpty()
{
    if(top == NULL)
    {
        return 1;
    }

    else
    {
        return 0;
    }
}