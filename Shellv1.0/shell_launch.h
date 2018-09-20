#include "display_home_converter.h"
#include "doneProcesses.h"
#include "get_input.h"
#include "tokenizer.h"
#include "execute.h"
#include "glob_var.h"




int shell_launch()
{
    char *input , **args ;
    int status  ;
   
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
            display[0] = home_sym;
            display[1] = '\0';
        }
        else
        {
            strcpy(display,curr_dir);
            display_home_converter(display,home_dir);
        }
        printf(CYAN "<" BLUE "%s" CYAN "@" YELLOW "%s" CYAN ":%s>" RESET,user_name,host_name,display);
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
            //do_in_background= background_proc(args);
            
             /*
            int xx = 0;
            while(args[xx]!=NULL)
            {
                printf("%s\n",args[xx]);
             xx++;
             }
            */ 

            status = execute(args,do_in_background);
            doneProcesses();    
            free(args);
            
            
        }
    free(commands);
    continue;
    } while (1);
    return 0;
    
}