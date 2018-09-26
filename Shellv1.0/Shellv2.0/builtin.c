/*************************** Functions for implementing built-in commands *************************************/

#include "header.h"


int cd(char **args )
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


void pwd(char** args) {
        printf("%s\n", curr_dir);
}

void jobs() {
        int i;
        for(i = 0; i < num_jobs ; i++) {
                if(table[i].active) {
                        printf("[%d] %s [%d]\n", i, table[i].name, table[i].pid);
                }
        }
}
void setemv(char **args)
{
    if(args[1] == NULL || args[2] == NULL)
        {
                perror(RED "Too few args to setenv" RESET);
                return;
        }
    char* val=args[2];
    char* var=args[1];
    setenv(val,var,1);
    printf("%s\n",getenv(val));
}
void unsetemv(char **args)
{
    if(args[1]==NULL){
        perror(RED "Too few args to unsetenv" RESET);
        return;


    }
    char* var =args[1];
    unsetenv(var);
}
void ls(char* opt){
    char    perm[11];
    char    *timestring;
    int     a = 0;
    int     l = 0;
    DIR     *dir;
    struct passwd   *pwd;
    struct group    *grp;
    struct dirent   *fil;
    struct stat     stt;
    if(opt && ((strlen(opt) == 2 && opt[1] == 'a') ||
            (strlen(opt) == 3 && (opt[1] == 'a' || opt[2] == 'a'))))
        a = 1;

    if(opt && ((strlen(opt) == 2 && opt[1] == 'l') ||
            (strlen(opt) == 3 && (opt[1] == 'l' || opt[2] == 'l'))))
        l = 1;

    dir = opendir(".");
    if(dir == NULL){
        strerror(errno);
        return;
    }

    while((fil = readdir(dir)) != NULL){
        if(a == 0 && fil->d_name[0] == '.') continue;

        if(l){
            lstat(fil->d_name, &stt);
            pwd = getpwuid(stt.st_uid);
            grp = getgrgid(stt.st_gid);
            timestring = ctime(&stt.st_mtime);

            timestring[strlen(timestring)-1] = '\0';
            
            filePermissionDisplay(stt, perm);
            printf("%s %d\t %s\t %s\t %jd\t", perm, (int)stt.st_nlink, pwd->pw_name, grp->gr_name, (intmax_t)stt.st_size);
            printf("%s\t", timestring+4);
            printf("%s\n", fil->d_name);
        }
        else printf("%s\n", fil->d_name);
    }
}
int kjob(char** args) {
        if(args[1] == NULL)
        {
                perror(RED "Insufficient args for killjobs. Please try again with the right syntax" RESET);
                return 1;
        }
        if(args[2]==NULL)
        {
                perror(RED "Insufficient args for killjobs. Please try again with the right syntax" RESET);
                return 1;
        }
       
        int job_num = atoi(args[1]);
        if(table[job_num].active ) {
                if(kill(table[job_num].pid, atoi(args[2])) < 0)
                        {perror(RED "Signal could not be sent \n" RESET);
                        return 1;
                        }
                return 0;
        }
        perror(RED "Error 404 , job not found \n" RESET);
        return 1;       
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

void overkill()
 {
        int i=0;
        while(i<num_jobs)
         {
                if(table[i].active ) 
                {
                        if(kill(table[i].pid, SIGKILL) < 0) 
                                perror( RED "Could not kill \n" RESET);
                }
                i++;
        }
}

int fg(char** args) {
        if( args[0]==NULL || args[1] == NULL) {
                perror(RED "Not enough arguemts for fg . Check Syntax " RESET);
                return 1;
        }       

        int i, job_num = atoi(args[1]), status;
        if(table[job_num].active == 0) {
                printf(RED "Error 404 Job not found \n" RESET);
                return 1;
        }
        int error_present = 1;
        if(table[job_num].active) {
                int pid = table[job_num].pid, pgid;
                pgid = getpgid(pid);
                error_present=0;
                tcsetpgrp(shell, pgid);
                fgpid = pgid;
                if(killpg(pgid, SIGCONT) < 0)
                        {
                                perror(RED "Cant bring it to forward\n" RESET);
                                error_present =1 ;
                        }
                waitpid(pid, &status, WUNTRACED);
                if(!WIFSTOPPED(status)) {
                        table[job_num].active--;
                        fgpid = 0;
                }
                tcsetpgrp(shell, my_pid);
        }
        else perror(RED "Error 404 Job not found \n" RESET );
        if(error_present)
                return 1;
        return 0;
}
int clck(char **args)
{
   if (args[1] == NULL)
        return 0;
    char fd[256] ;
    char path [] = "/proc/driver/rtc" ;
    FILE *status = fopen(path,"r");

    if (status == NULL)
    {
        perror (RED "clck file open error" RESET);
        return 1;
    }
    int  i = 0,num;
    num = atoi(args[2]);
    while(i<num)
    {
        
        status = fopen(path,"r");
        if ( status== NULL)
        {
        perror (RED "clck file open error" RESET);
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
