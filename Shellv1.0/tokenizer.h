#include "glob_var.h"

char ** tokenizer(char *line , int type)
{
    //printf("Called\n"); 
    int size = BUFF_SIZE;
    char ** tokens = malloc(sizeof(char*)*size);
    char *token , *delimiter;
    if ( type == 1)
    {
        delimiter = ";";
    }
    if (type == 2)
    {
        delimiter = " \t\r\n\v\a";
    }
    int i;
    if (tokens == NULL)
    {
        perror(RED "Allocation Error \n" RESET);
        exit(EXIT_FAILURE);
    }

    token = strtok(line,delimiter);
    for(i=0;;line=NULL)

    {
       
       
       tokens[i]= token;
       if(token == NULL)
       {
           break;
       }
       i++;
       token = strtok(NULL,delimiter);
       if (i>= size)
       {
           size+= size ;
           tokens=realloc(tokens,sizeof(char*)*size);
           if(tokens == NULL)
           {
                perror(RED "Allocation Error \n" RESET);
                exit(EXIT_FAILURE);
           }

       }
    }
    return tokens;
}