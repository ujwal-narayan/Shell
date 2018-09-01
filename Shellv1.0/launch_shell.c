#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define RED "\033[0;31m"
#define RESET "\033[0m"

int BUFF_SIZE = 100 ; 



char ** tokenizer(char *line , int type)
{
    //printf("Called\n"); 
    int size = BUFF_SIZE;
    char ** tokens = malloc(sizeof(char*)*size);
    char *token , *delimiter
    if(type == 1)
    {
    delimiter = ";";
    }
    else if(type == 2)
    {
        delimiter = " \t\r\n\v\f";
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
int display_home_converter(char display[], char home_dir[])
{
    int size = strlen(display) , i , j=1;
    char temp[size+1];
    int is_parent_of_home = 0;

    if(size  < strlen(home_dir))
        return 0;

    for (i = 0 ; i< size ;i++ )
    {
        
        if (display[i] == home_dir[0])
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

int shell_launch(int start)
{
    char *input , **args ;
    int status  ;
    int do_in_background = 0;
    char curr_dir[BUFF_SIZE], host_name[BUFF_SIZE] , user_name[BUFF_SIZE];
    int display_size = BUFF_SIZE*3 ,i , j;
    char display[display_size];
    char home_dir[BUFF_SIZE];
    char* line;
    char **commands;
    if (getcwd(home_dir,BUFF_SIZE)==NULL)
        {
            perror(RED "Error while finding the current directory\n" RESET);
            return 1;
        }
    
    do
    {
        if (getcwd(curr_dir,BUFF_SIZE)==NULL)
        {
            perror(RED "Error while finding the current directory\n" RESET);
            return 1;
        }
        if (gethostname(host_name,BUFF_SIZE)==-1)
        {
            perror(RED "Error in finding hostname\n" RESET);
            return 1;
        }
        if (getlogin_r(user_name,BUFF_SIZE)!=0)
        {
            perror(RED "Error in finding the username\n" RESET);
            return 1;
        }
        if(strcmp(home_dir , curr_dir) == 0)
        {
            display[0] = '~';
            display[1] = '\0';
        }
        else
        {
            strcpy(display,curr_dir);
            display_home_converter(display,home_dir);
        }
        printf("<%s@%s:%s>",user_name,host_name,display);
        line = get_input();
        if(line == NULL)
        {
             perror(RED "Error while taking the input \n" RESET);
             return 1;
        }
        
        commands = tokenizer(line,1);
       // printf("DONE\n");
       /*
        int xx = 0;
        while(commands[xx]!=NULL)
        {
            printf("%s\n",commands[xx]);
            xx++;
        }*/
        
        for(i=0;commands[i]!=NULL;i++)
        {
            do_in_background = 0; 
            

        }
        free(line);
        free(commands);

        

        

    
    } while (status);
    return 0;
    
}

int main(int argc , char* argv[])
{
    int ret;
    ret = shell_launch(0);
    return ret;
}   