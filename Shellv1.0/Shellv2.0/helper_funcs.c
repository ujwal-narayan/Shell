#include "headers.h"
int display_home_converter(char display[], char home_dir[])
{
    int size = strlen(display) , i , j=1;
    char temp[size+1];
    int is_parent_of_home = 0;

    if(size  < strlen(home_dir))
        return 0;

    for (i = 0 ; home_dir[i]!='\0' ;i++ )
    {
        
        if (display[i] == home_dir[i])
            continue;
        else 
        {
            is_parent_of_home = 1 ;
            break;

        }

    }
    if (is_parent_of_home == 1)
        return 0;
    temp[0]= '~';
    while (i<size)
    {
        temp[j] = display[i];
        j++;
        i++;
    }
    strcpy (display , temp );
    return 0;
    
}
int strstrlen(char **arr)
{
    int i = 0;
    while(arr[i]!=NULL)
    {
        i++;
    }
    return i;
}
char** tokenizer(char *line , int type)
{
    //printf("Called\n"); 
    int size = MAX_BUF_LEN;
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
char *readLine()
{
        int bufsize = MAX_BUF_LEN, pos = 0, ch;
        char *buffer = (char *)malloc(sizeof(char) * bufsize);
        if (!buffer)
                perror(RED "Buffer could not be declared " RESET);
        while (1)
        {
                ch = getchar();
                if (ch == EOF || ch == '\n')
                {
                        buffer[pos++] = '\0';
                        return buffer;
                }
                else
                        buffer[pos++] = ch;
                if (pos >= bufsize)
                {
                        bufsize *= MAX_BUF_LEN;
                        buffer = (char *)realloc(buffer, bufsize);
                        if (!buffer)
                                 perror(RED "Buffer could not be declared " RESET);
                }
        }
}