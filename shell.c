#define MAX 1000000
#include <stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>



//GLobal Vars

char host_name[MAX],user_name[MAX], home_dir[MAX], curr_dir[MAX];

int string_compare(char a[] , char b[])
{
    
    for(int i = 0;a[i]!='\0';i++)
    {
        if(a[i]!=b[i])
            return 1;
    }
    return 0;
}

char **shell_split_line(char *line )
{
    int size = MAX;
    char **tokens = malloc(sizeof(char)*size);
    if (tokens == NULL)
    {
        perror("Buffer allocation error \n");
        exit(1);
    }
    char *token ; 

    token = strtok(line," \t\r\n\f\v");
    int pos = 0;
    for(token = strtok(line," \t\r\n\f\v");token != NULL;token = strtok(line," \t\r\n\f\v"))
    {
        tokens[pos] = token;
        pos++;
        if(pos >= size)
        {
            size+=MAX;
            tokens = realloc(tokens,sizeof(char)*size);
            if (tokens == NULL)
            {
                perror("Buffer allocation error \n");
                exit(1);
            }
        }
    }
    tokens[pos] = token;
    return tokens;

}
char *shell_read_line()
{
    int pos = 0;
    int size = MAX;
    char *buffer = malloc(sizeof(char)*size);
    int input;
    if(buffer == NULL)
    {
        perror("Buffer allocation error \n");
        exit(1);
    }
    while(1)
    {
        input = getchar();
        if (input == EOF)
        {
            buffer[pos] = '\0';
            return buffer;
        }
        
        if (input == '\n')
        {
            buffer[pos] = '\0';
            return buffer;
        }
        buffer[pos] = input;
        pos++;

        if (pos >= size)
        {
            size= size + MAX;
            buffer = realloc(buffer,sizeof(char)*size);
            if(buffer == NULL)
            {
                perror("Buffer allocation error \n");
                exit(1);
            }


        }
    }



}
int shell_loop()
{
    char *line ;
    char **args ;
        
    while(1)
    {
         if(getcwd(curr_dir,MAX) ==NULL)
        {
            perror("Error in getting the current directort ");
            return 1;
        }


        if(string_compare(curr_dir,home_dir) == 0)
        {
            printf("<%s@%s:~>",user_name,host_name);
        }
        else 
        {  
            printf("<%s@%s:%s>",user_name,host_name,curr_dir);
        }
        
        line = shell_read_line();
        args  = shell_split_line(line);
         if(shell_exec(args) == 0 )
        {
            return 0;

        }
        
        free(line);
        free(args);
    }
    
}

int main(int argc , char **argv )
{
    
    int host_name_ret , user_name_ret ;
    char* home_dir_ret ;
    host_name_ret =  gethostname(host_name, MAX);
    user_name_ret =  getlogin_r(user_name,MAX);
    home_dir_ret = getcwd(home_dir,MAX);
   


    if (host_name_ret || user_name_ret || home_dir_ret == NULL)
    {
        perror("Error in hostname or username or homedir ");
        return 1;
    }
    shell_loop();
    return 0;
}