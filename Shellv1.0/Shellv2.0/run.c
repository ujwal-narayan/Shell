#include "header.h"
void signal_inti(int p);

int run_cmd(char** cmd_tokens) {
        pid_t pid;
        pid = fork();
        int error_present = 0;
        if(pid < 0) {
                perror(RED "The Child Process could not be created \n" RESET );
                error_present = 1;
                return -1; 
        }
        else if(pid==0) { 
                int f_in, f_out;
                int pid_cpy = pid;
                setpgid(pid, pid); 
                
                if(input_redi) 
                {
                        f_in = open_infile();
                        if(f_in < 0)
                        {
                                _exit(-1);
                        }

                }
                if(output_redi) 
                {
                        f_out = open_outfile();
                      if(f_out < 0)
                      {
                              _exit(-1);
                      } 
                }

                if(!is_bg ) tcsetpgrp(shell, getpid());  
                signal_inti(pid);
                int ret;
                if((ret = execvp(cmd_tokens[0], cmd_tokens)) < 0) {
                        
                        perror(RED "An error occured while executing!\n" RESET);
                        error_present = 1;
                        _exit(-1);
                }
                _exit(0);
        }
        if(!is_bg) 
        {
                tcsetpgrp(shell, pid);
                int status;
                add_proc(pid, cmd_tokens[0]);
                
                fgpid = pid;
                waitpid(pid, &status, WUNTRACED);
                
                if(!WIFSTOPPED(status)) 
                {
                        rem_proc(pid); 
                }
         
                else 
                {
                        error_present = 0;
                        fprintf(stderr, "\n%s with pid %d has stopped!\n", cmd_tokens[0], pid);
                }
                
                tcsetpgrp(shell, my_pgid); 
        }
        else 
        {
                add_proc(pid, cmd_tokens[0]);
                printf("[%d] %d\n", num_jobs, pid);
                return 0;
        }
}

void signal_inti(int p )
{
                if (p!=0)
                        return;
                signal (SIGINT, SIG_DFL);
                signal (SIGQUIT, SIG_DFL);
                signal (SIGTSTP, SIG_DFL);
                signal (SIGTTIN, SIG_DFL);
                signal (SIGTTOU, SIG_DFL);
                signal (SIGCHLD, SIG_DFL);
                return ;
}
void add_proc(int pid, char* name)
 {
        table[num_jobs].pid = pid;
        table[num_jobs].active = 1;
        table[num_jobs].name = strdup(name);
        num_jobs++;
}

void rem_proc(int pid) 
{
        int i=0;
        for(i=0; i < num_jobs; i++) {
                if(table[i].pid == pid) {
                        table[i].active = 0;
                        break;
                }
        }
}
int  execute_buitlin( char** cmd_tokens, char* cmd_copy) 
{
        if(strcmp(cmd_tokens[0],"remindme\0")==0) remindme(cmd_tokens);
        else if(strcmp(cmd_tokens[0], "cd\0") == 0)
        {
                 cd(cmd_tokens);
        }
        else if(strcmp(cmd_tokens[0], "kjob\0") == 0) kjob( cmd_tokens);
        else if(strcmp(cmd_tokens[0], "jobs\0") == 0) jobs();
        else if(strcmp(cmd_tokens[0], "pinfo\0") == 0) pinfo(cmd_tokens);
        else if(strcmp(cmd_tokens[0], "pwd\0") == 0) pwd(cmd_tokens);
        else if(strcmp(cmd_tokens[0], "overkill\0") == 0) overkill();
        else if(strcmp(cmd_tokens[0], "echo\0") == 0) echo( cmd_copy);
        else if(strcmp(cmd_tokens[0],"clock\0")==0) clck(cmd_tokens);
        else if(strcmp(cmd_tokens[0],"setenv\0")==0) setemv(cmd_tokens);
        else if(strcmp(cmd_tokens[0], "fg\0") == 0 ) fg( cmd_tokens);
        else if(strcmp(cmd_tokens[0],"unsetenv\0")==0) unsetemv(cmd_tokens);
        else if(strcmp(cmd_tokens[0],"ls")==0) {
               char * element = cmd_tokens[1];
                char *temp = cmd_tokens[2];
        if(element && (temp || !(!strcmp("-la", element) || !strcmp("-a", element)
                        || !strcmp("-l", element) || !strcmp("-al", element)))){
            return 1;
        }
        else
            ls(element);
         }
        else if(strcmp(cmd_tokens[0], "quit\0") == 0) _exit(0);
        else
                return 1;
        return 0;

}

void normal_cmd(int tokens, char** cmd_tokens, char* cmd_copy) 
{
        if(tokens > 0)
         {
                if(strcmp(cmd_tokens[tokens-1], "&\0") == 0) 
                {
                        is_bg = 1;
                        cmd_tokens[tokens - 1] = NULL;
                        run_cmd(cmd_tokens); 
                }
                else 
                {
                        int is_not_builtin =0;
                is_not_builtin = execute_buitlin(cmd_tokens, cmd_copy);
                
                if(is_not_builtin)
                        run_cmd(cmd_tokens);
                }
        }
        free(cmd_tokens);
}

void redi_and_pipi_cmd(char* cmd) {
        int pid, pgid, f_in, f_out;
        char cmd_name[MAX_BUF_LEN];
        num_pipe = 0;
        int is_error=0;
        char* copy = strdup(cmd);
        char* token;
        token = strtok(copy, pipe_str);
        while(token!= NULL) {
                pipe_cmds[num_pipe++] = token;
                token = strtok(NULL, pipe_str);
        }

        int* pipes = (int* )malloc(sizeof(int)*(2*(num_pipe - 1)));

        int i , status,j;;

        for(i = 0; i < 2*num_pipe - 3; i += 2) {
                if(pipe(pipes + i) < 0 ) 
                {   
                        perror(RED "Pipe could not opened!\n" RESET);
                        is_error++;
                        return;
                }
        }
        
        for(i = 0; i < num_pipe ; i++) 
        {

                char** cmd_tokens = malloc((sizeof(char)*MAX_BUF_LEN)*MAX_BUF_LEN); 
                is_bg = 0;    
                int tokens = parse_for_redi(strdup(pipe_cmds[i]), cmd_tokens);
                strcpy(cmd_name,cmd_tokens[0]);        
                pid = fork();
                if(i < num_pipe - 1){
                     add_proc(pid, cmd_name);
                }
                
                is_error = 0;
                if(pid != 0 )
                 {
                        if(i == 0 ) pgid = pid;
                        setpgid(pid, pgid);  
                }
                if(pid < 0)
                 {
                        perror(RED "An error occured while Forkin !\n" RESET);
                        is_error ++;
                }
                else if(pid == 0) 
                {
                        signal_inti(pid);


                        if(input_redi) {
                                f_in = open_infile();
                        }
                        else if(i > 0 ) 
                        {
                                dup2(pipes[2*i -2], 0);
                        }
                        
                        if(output_redi) 
                        {
                                f_out = open_outfile();
                        }
                        else if(i < num_pipe - 1) 
                        {
                                dup2(pipes[2*i + 1], 1);
                        }

                 
                        for(j = 0; j < 2*num_pipe - 2; j++) close(pipes[j]);
         
                        if(execvp(cmd_name, cmd_tokens) < 0 )
                         {
                                is_error++;
                                perror(RED "An error occured while running execvp!\n" RESET);
                                _exit(-1);
                        }
                }
        }
 
        for(i = 0; i < 2*num_pipe - 2; i++) close(pipes[i]);
 
        if(!is_bg) 
        {
                tcsetpgrp(shell, pgid);
                i = 0;
                while(i<num_pipe)
                {
                        int cpid = waitpid(-pgid, &status, WUNTRACED);
                        if(!WIFSTOPPED(status))
                         {
                                rem_proc(cpid);
                         }
                         i++;
                }
                tcsetpgrp(shell, my_pgid); 
        }
}
