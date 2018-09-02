#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define RED "\033[0;31m"
#define RESET "\033[0m"

int BUFF_SIZE = 100 ; 


char home_dir[100],prev_dir[100],curr_dir[100];
int cd(char **args)
{
    int ret ;
    if(args[1]==NULL || strcmp(args[1],"~")==0)
    {
        ret = chdir(home_dir);
        if (ret == -1)
           {
               perror(RED "cd error" RESET);
           }
        return ret;
    }
    if (strcmp(args[1],"-")==0)
    {
        printf("hi\n");
        ret = chdir(prev_dir);
        return ret;
    }
    ret = chdir(args[1]);
    if (ret == -1)
    {
        perror(RED "cd error" RESET);
    }
    strcpy(prev_dir,curr_dir);
    return ret;
}

int pwd()
{
    return 0;
}

int echo(char **args)
{
    return 0;
}
int pinfo(char **args)
{
    return 0;
}


int ls(char **args)
{
    return 0;
}

int run(char **args,int do_in_background)
{
    return 0;
}

int execute_builtin(char **args)
{
    //printf("Hi %s\n",args[0]);
    if(strcmp(args[0],"cd")==0)
    {
        //printf("Hi\n"); 
        
        return cd(args);
    }
    else if (strcmp(args[0],"pwd")==0)
    {
        return pwd();
    }
    else if (strcmp(args[0],"echo")==0)
    {
        return echo(args);
    }
    else if (strcmp(args[0],"pinfo")==0)
    {
        return pinfo(args);
    }
    else if (strcmp(args[0],"ls")==0 )
    {
        return ls(args);
    }

    return -73;


}

int execute(char **args , int do_in_background)
{
    int builtin;
    
    if(args[0]==NULL)
    {
       // printf("Come on , plis work\n");
        return 1;
    }
    //printf("HI ,args : %s\n",args[0]);;
    builtin = execute_builtin(args);
    if(builtin == -73)
       {
            return run(args,do_in_background);
       }
    else 
        return builtin;
}

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

int background_proc(char** args )
{
    int size = 0;
    
    while(args[size]!=NULL)
    {
        size++;
    }
    if(size == 0)
        return -1;
    int last_char = strlen(args[size-1])-1;
    if(args[size-1][last_char] == '&')
        {
            args[size-1][last_char]=' ';
            return 1;
        }
    else
        return 0;

}
int shell_launch(int start)
{
    char *input , **args ;
    int status  ;
    int do_in_background = 0;
    char  host_name[BUFF_SIZE] , user_name[BUFF_SIZE];
    int display_size = BUFF_SIZE*3 ,i , j;
    char display[display_size];
    
    char* line;
    char **commands;
    if (getcwd(home_dir,BUFF_SIZE)==NULL)
        {
            perror(RED "Error while finding the current directory\n" RESET);
            return 1;
        }
    strcpy(prev_dir,home_dir);
    
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
        printf("\n%s \n%s\n",prev_dir,curr_dir);
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
        }
        continue;
        */
        for(i=0;commands[i]!=NULL;i++)
        {
            args = tokenizer(commands[i],2);
            do_in_background= background_proc(args);
            
             /*
            int xx = 0;
            while(args[xx]!=NULL)
            {
                printf("%s\n",args[xx]);
             xx++;
             }
            */ 

            status = execute(args,do_in_background);
            free(args);
            
        }
    free(commands);
    continue;
    } while (1);
    return 0;
    
}

int main(int argc , char* argv[])
{
    int ret;
    ret = shell_launch(0);
    return ret;
}   