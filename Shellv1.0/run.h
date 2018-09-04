#include "glob_var.h"

int run(char **args,int bg)
{
        if (args[0] == NULL)
                return 1;
      
        

        int len = strstrlen(args);
        int i;
        pid_t pid;
        int status;

        pid = fork();
        if (pid == 0)
        {
                //Child 
                if (strcmp(args[len - 1], "&") == 0)
                        args[len - 1] = "";
               
                if (execvp(args[0], args) == -1)
                       {
                        
                        return 1;
                       } 
                exit(EXIT_FAILURE);
        }
        else if (pid < 0)
                {
                    perror(RED "Error forking" RESET);
                    return 1;
                }   
        else
        {
                //Parent 
                if (strcmp(args[len - 1], "&") != 0)
                {
                    do
                        {
                                waitpid(pid, &status, WUNTRACED);
                        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
                       
                }
                else
                {
                     //printf("Process ID of %s: %d\n", args[0], pid);
                        int i;
                        for (i = 0; i < CHILDPROCESSESLEN; i++)
                                if (childProcesses[i] == -73)
                                {
                                        childProcesses[i] = pid;
                                        strcpy(childNames[i], args[0]);
                                        break;
                                }
                        waitpid(pid, &status, WNOHANG);
                        
                }
        }
        return 1;
}