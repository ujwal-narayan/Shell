#include<stdio.h>



char* get_input()
{
    char* line = NULL ;
    size_t len ;
    ssize_t read;
    read = getline(&line,&len,stdin);
    if(read == -1)
    {
       
        return NULL;
    }
    else
    {
        return line;

    }

}