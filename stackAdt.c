#include "stackAdtDef.h"

// function definitions

void s_push(stackElement* element)
{
    //creating back link
    element->link = s_top;
    
    // change top
    s_top  = element;
}

void s_pop()
{
    stackElement* new_top = s_top;
    //update top
    s_top = s_top->link;
    // remove the top
    free(new_top);
}

int isEmpty()
{
    if(s_top == NULL)
    {
        return 1;
    }

    else
    {
        return 0;
    }
}


