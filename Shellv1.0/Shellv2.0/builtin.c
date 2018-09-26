/*************************** Functions for implementing built-in commands *************************************/

#include "header.h"


int cd_cmd(char **args , char* cwd , char* home_dir)
{
    
    
    int ret ;
    if(args[1]==NULL || strcmp(args[1],"~")==0)
    {
        strcpy(prev_dir,curr_dir);
        ret = chdir(home_dir);
        
        if (ret == -1)
           {
               perror(RED "cd error" RESET);
           }
        return ret;
    }
    
    if (strcmp(args[1],"-")==0)
    {
        //printf("hi\n");
        char * tempa = curr_dir;
        ret = chdir(prev_dir);
        if (ret == -1)
           {
               perror(RED "cd error" RESET);
               return ret;
           }
        strcpy(prev_dir,tempa);
       // printf("%s %s\n",prev_dir,curr_dir);
        return ret;
    }
    strcpy(prev_dir,curr_dir);
    ret = chdir(args[1]);
   
    if (ret == -1)
    {
        perror(RED "cd error" RESET);
    }
   
    return ret;
}
int echo(char *line)
{
        int i;
        int flg1 = 0;
        int flg2 = 0;
        int bufsize = MAX_BUF_LEN, pos = 0, ch;
        char *buffer = (char *)malloc(sizeof(char) * bufsize);
        line = &line[5];
        do
        {
                int len = strlen(line);
                for (i = 0; i < len; i++)
                {
                        ch = line[i];
                        if (ch == '\'')
                        {
                                if (flg2)
                                        buffer[pos++] = ch;
                                else
                                        flg1 = !flg1;
                        }
                        else if (ch == '\"')
                        {
                                if (flg1)
                                        buffer[pos++] = ch;
                                else
                                        flg2 = !flg2;
                        }
                        else
                                buffer[pos++] = ch;
                        if (pos >= bufsize)
                        {
                                bufsize *= MAX_BUF_LEN;
                                buffer = (char *)realloc(buffer, bufsize);
                        }
                }
                if (flg1 || flg2)
                {
                        printf("> ");
                        line = readLine();
                }
                buffer[pos++] = '\n';
                // strcat(friend, "\n");
        } while (flg1 || flg2);
        buffer[pos] = 0;
        printf("%s", buffer);
        free(buffer);
        buffer = NULL;
        return 1;
}


void pwd(char** cmd_tokens) {
        if(cmd_tokens[1] == NULL) printf("%s\n", curr_dir);
        else run_cmd(cmd_tokens);
}

void jobs() {
        int i;
        for(i = 0; i < num_jobs ; i++) {
                if(table[i].active == 1) {
                        printf("[%d] %s [%d]\n", i, table[i].name, table[i].pid);
                }
        }
}

void kjob(char** cmd_tokens) {
        if(cmd_tokens[0] == NULL || cmd_tokens[1] == NULL || cmd_tokens[2] == NULL) {
                fprintf(stderr, "Invalid usage of kjob!\n");
                return;
        }
        int job_num = atoi(cmd_tokens[1]);
        if(table[job_num].active == 1) {
                if(kill(table[job_num].pid, atoi(cmd_tokens[2])) < 0)                 /* For sending signal mentioned to job mentioned */
                        fprintf(stderr, "Signal not sent!\n");
        }
        else fprintf(stderr, "Job not found\n");               
}
int remindme(char** args)
{
    char* timer = args[1];
    char mes[MAX_BUF_LEN] = "\0";
    for (int i =2;args[i]!=NULL;i++)
    {
            strcat(mes,args[i]);
            strcat(mes," \0");
    }

    int time = atoi(timer);
    int pid = fork();
    if(pid == 0)
    {
        sleep(time);
        printf("\nReminder: %s \n", mes);
    }
    return 0;
}

void overkill() {
        int i;
        for(i = 0 ; i < num_jobs ; i++) {
                if(table[i].active == 1) {
                        if(kill(table[i].pid, SIGKILL) < 0)                           /* Kill all jobs */
                                perror("Error killing process!\n");
                }
        }
}

void fg(char** cmd_tokens) {
        if( cmd_tokens[0]==NULL || cmd_tokens[1] == NULL) {
                fprintf(stderr, "Invalid usage of fg");
                return;
        }       

        int i, job_num = atoi(cmd_tokens[1]), status;
        if(table[job_num].active == 0) {
                printf("No such job exists\n");
                return;
        }
        if(table[job_num].active == 1) {
                int pid = table[job_num].pid, pgid;
                pgid = getpgid(pid);                     /* get pgid of mentioned job */
                tcsetpgrp(shell, pgid);                  /* Give control of shell's terminal to this process */

                fgpid = pgid;                            /* Set this pgid as fg pgid */
                if(killpg(pgid, SIGCONT) < 0)            /* Send signal to thid pgid to continue if stopped */
                        perror("Can't get in foreground!\n");
                waitpid(pid, &status, WUNTRACED);        /* Wait for this process, return even if it has stopped without trace */
                if(!WIFSTOPPED(status)) {                /* returns true if the child process was stopped by delivery of a signal */
                        table[job_num].active = 0;
                        fgpid = 0;
                }
                tcsetpgrp(shell, my_pid);                /* Give control of terminal back to the executable */
        }
        else fprintf(stderr, "No job found\n");
}
int clock(char **args)
{
   if (args[1] == NULL)
        return 0;
    char fd[256] ;
    char path [] = "/proc/driver/rtc" ;
    FILE *status = fopen(path,"r");

    if (status == NULL)
    {
        perror (RED "Clock file open error" RESET);
        return 1;
    }
    int  i = 0,num;
    num = atoi(args[2]);
    while(i<num)
    {
        
        status = fopen(path,"r");
        if ( status== NULL)
        {
        perror (RED "Clock file open error" RESET);
        return 1;
        }
        
    char* ret = fgets(fd,256,status);
    if (ret == NULL)
    {
        perror(RED "fgest error" RESET);
        return 1;
    }
    char** asd= tokenizer(fd,2);
    printf("%s , ",asd[2]);
    ret = fgets(fd,256,status);
    if (ret == NULL)
    {
        perror(RED "fgest error" RESET);
        return 1;
    }
    asd= tokenizer(fd,2);
    printf("%s\n",asd[2]);
    sleep(num);
    fclose(status);
    i++;

    }
    return 0;
}
