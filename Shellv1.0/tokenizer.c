#include<string.h>
#include<stdlib.h>
#include<stdio.h>

char ** tokenizer(char *line )
{
    //printf("Called\n"); 
    int size = BUFF_SIZE;
    char ** tokens = malloc(sizeof(char*)*size);
    char *token  ;
    char *delimiter = ";";
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