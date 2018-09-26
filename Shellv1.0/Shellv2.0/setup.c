
#include "header.h"

void signal_handler(int signum) {
       
        if(signum == SIGCHLD) {                  /* For handling signal from child processes */
                int i=0;
                int pid_quit=0;
                int status = 0;
                while( (pid_quit = waitpid(-1, &status, WNOHANG)) > 0) {  

                        while(i<num_jobs)
                        {
                                if(table[i].pid == pid_quit)
                                        break;
                                i++;
                                
                        }
                        if(i == num_jobs)
                                continue;
                        else if (i < num_jobs) {
                        table[i].active = 0;
                        if(WIFSIGNALED(status))   /* returns true if the child process was terminated by a signal */
                                printf("\n%s with pid %d has been forced to close \n", table[i].name, table[i].pid);        
                        else if(WIFEXITED(status))          /* returns true if the child terminated normally */
                                printf("\n%s with pid %d exited normally\n", table[i].name, table[i].pid);
                        
                       
                        }
                        else
                                continue;
                }
        }
        else if (signum == SIGINT)
         {
                signal(SIGINT,SIG_IGN);               /* For ignoring ctrl + c */
                signal(SIGINT, signal_handler);       /* For re-setting signal handler */
        }
}

