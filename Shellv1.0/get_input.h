#include "glob_var.h"
char* get_input()
{
    char* line = NULL ;
    size_t len ;
    ssize_t read;
    read = getline(&line,&len,stdin);
    if(read == -1)
    {
        perror(RED "Error while taking the input \n" RESET);
        return NULL;
    }
    else
    {
        return line;

    }

}
