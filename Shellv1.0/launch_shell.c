

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <grp.h>
#include <termios.h>
#include <assert.h>
#include <errno.h>
#include<fcntl.h>

#define RED "\033[0;31m"
#define RESET "\033[0m"
#define CHILDPROCESSESLEN 128

int BUFF_SIZE = 100 ; 

//FIX RUN : PRINTING ERROR CODE FOR BG PROX
//PINFO , LS , RUN ,HANDLER


pid_t childProcesses[CHILDPROCESSESLEN] = {};
char childNames[CHILDPROCESSESLEN][256] = {};

int strstrlen(char **arr)
{
    int i = 0;
    while(arr[i]!=NULL)
    {
        i++;
    }
    return i;
}

/*
""""
An attempt to keep a history of directories a user went to so that instead of just the previous directory the user can go to n previous directories.
Sadly due to time constraints and mid sems could not be implemented . Might implement in the future if I ever get back to this , Highly doubtful but who knows 
""""


struct Stack
{
    int top;
    unsigned capacity ;
    char** array;

};
//stack supposed to help me implent cd - 
struct Stack* createStack(unsigned capacity)
{
    struct Stack* stack = (struct Stack*) malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (char**) malloc(stack->capacity * sizeof(char*));
    return stack;
}
 
// Stack is full when top is equal to the last index
int isFull(struct Stack* stack)
{   return stack->top == stack->capacity - 1; }
 
// Stack is empty when top is equal to -1
int isEmpty(struct Stack* stack)
{   return stack->top == -1;  }
 
// Function to add an item to stack.  It increases top by 1
void push(struct Stack* stack, char* item)
{
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;
    printf("%s pushed to stack\n", item);
}
 
// Function to remove an item from stack.  It decreases top by 1
char* pop(struct Stack* stack)
{
    if (isEmpty(stack))
        return NULL;
    stack->top--;
    return stack->array[stack->top-1];
}
struct Stack* stack;
*/
char home_dir[100],prev_dir[100],curr_dir[100];
 int do_in_background = 0;


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
int cd(char **args)
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

int pwd()
{
    printf("%s\n",curr_dir);
    return 0;
}

int echo(char **args)
{
   int i;   
   for(i=1;args[i]!=NULL;i++)
   {
       printf("%s ",args[i]);
   }
   printf("\n");
   return 0;
}
int pinfo(char **args)
{
        int c, L = 0;
        pid_t Id;

        //2 cases, one passable argument
        //Clear buffer space before this

        if (args[1] == NULL)
                Id = getpid();

        else
                Id = atoi(args[1]);

        char Reuse_p[1024];
        char buffer[1024];

        printf("pid -- %d\n", Id);

        sprintf(Reuse_p, "/proc/%d/status", Id); //pass value

        FILE *fp = fopen(Reuse_p, "r");
        if (fp==NULL)
        {

                perror(RED "Error: could not open proc/pid/status file\n" RESET);
        }
        else
        {
                char status;
                do
                {
                        fgets(buffer, 256, fp);
                } while (buffer[0] != 'S');
                sscanf(buffer, "State:\t%c", &status);

                printf("Process Status -- %c\n", status);
                fclose(fp);
        }

        sprintf(Reuse_p, "/proc/%d/statm", Id);
        fp = fopen(Reuse_p, "r");
        if (fp==NULL)
        {

                perror(RED "Error: could not open proc/pid/statm file\n" RESET);
        }
        else
        {
                while ((c = fgetc(fp)) != ' ')
                        buffer[L++] = c;

                buffer[L] = '\0';

                printf("Memory -- %s\n", buffer);
                fclose(fp);
        }

        sprintf(Reuse_p, "/proc/%d/exe", Id);
        L = -4;
        L = readlink(Reuse_p, buffer,1023);
        if (L == -4)
        {

                perror(RED "Error: could not open proc/pid/exe file\n" RESET);
        }
        else
        {
                buffer[L] = '\0';       

                //mod_cwd_rel(buffer);
                display_home_converter(buffer,home_dir);
                printf("Executable Path -- %s\n", buffer);
        }
        return 0;
    
}
int ls(char **args)
{
        DIR *mydir;
        struct dirent *myfile;
        struct stat fileStat;
        int flagL, flagA, count;
        count = flagL = flagA = 0;
        int s = strstrlen(args);
        int i;
        for (i = 1; i < s; i++)
        {
                if (strcmp(args[i], "-l") == 0)
                {
                        flagL = 1;
                        count++;
                }

                if (strcmp(args[i], "-a") == 0)
                {
                        flagA = 1;
                        count++;
                }

                if (strcmp(args[i], "-la") == 0 || strcmp(args[i], "-al") == 0)
                {
                        flagA = 1;
                        flagL = 1;
                        count++;
                }
        }

        if (s - count == 1)
        {
                mydir = opendir("./");
                if(mydir == NULL)
                {
                    perror( RED "Error in opening the directory\n" RESET);
                    return 0;
                }
                if (flagL == 1)
                {
                        // if(s-count>1)
                        //   printf("%s:\n",args[i]);
                        char buf[512];
                        while ((myfile = readdir(mydir)) != NULL)
                        {
                                if (flagA == 0)
                                {
                                        if (myfile->d_name[0] == '.')
                                                continue;
                                }
                                int ret = sprintf(buf, "%s/%s", "./", myfile->d_name);
                                if(ret < 0 )
                                {
                                    perror(RED "Error while printing\n" RESET);
                                    return 1;
                                }
                                stat(buf, &fileStat);

                                struct group *user_group = getgrgid((long)fileStat.st_gid);
                                struct passwd *user_name = getpwuid((long)fileStat.st_uid);

                                printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
                                printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
                                printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
                                printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
                                printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
                                printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
                                printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
                                printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
                                printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
                                printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");

                                printf("  %lu", fileStat.st_nlink);

                                printf(" %4s %4s ", user_name->pw_name, user_group->gr_name);

                                printf("%lu", fileStat.st_size);

                                char *foo = ctime(&fileStat.st_ctime);
                                foo[strlen(foo) - 1] = 0;

                                printf(" %s", foo);

                                printf(" %s \n", myfile->d_name);

                                // free(foo);
                                // free(user_group);
                                // free(user_name);
                        }
                }

                else
                {
                        // if(s-count>1)
                        // printf("%s:\n",args[i]);
                        //char buf[512];
                        while ((myfile = readdir(mydir)) != NULL)
                        {
                                if (flagA == 0)
                                {
                                        if (myfile->d_name[0] == '.')
                                                continue;
                                }
                                //  sprintf(buf, "%s/%s", argv[1], myfile->d_name);
                                //  stat(buf, &fileStat);
                                printf(" %s \n", myfile->d_name);
                        }
                }
        }

        else
        {
                int i;
                for (i = 1; i < s; i++)
                {
                        //  printf("else condition");
                        if (args[i][0] == '-')
                        {
                                //printf("%s\n",args[i]);
                                continue;
                        }
                        //  printf("not continue %s \n",args[i]);
                        char tmp[512] = "";
                        if (args[i][0] == '~')
                        {
                                const struct passwd *pw = getpwuid(getuid());
                                const char *HOME = pw->pw_dir;
                                strcat(tmp, HOME);
                                strcat(tmp, &args[i][1]);
                                mydir = opendir(tmp);
                                if(mydir == NULL)
                                {
                                    perror( RED "Error in opening the directory\n" RESET);
                                    return 0;
                                }
                                
                        }
                        else
                                mydir = opendir(args[i]);
                                if(mydir == NULL)
                                {
                                    perror( RED "Error in opening the directory\n" RESET);
                                    return 0;
                                }

                        //  printf("seg ?");

                        if (flagL == 1)
                        {
                                if (s - count > 2)
                                        printf("%s:\n", args[i]);
                                char buf[512];
                                while ((myfile = readdir(mydir)) != NULL)
                                {
                                        if (flagA == 0)
                                        {
                                                if (myfile->d_name[0] == '.')
                                                        continue;
                                        }
                                        if (args[i][0] == '~')
                                                sprintf(buf, "%s/%s", tmp, myfile->d_name);
                                        else
                                                sprintf(buf, "%s/%s", args[i], myfile->d_name);
                                        stat(buf, &fileStat);

                                        struct group *user_group = getgrgid((long)fileStat.st_gid);
                                        struct passwd *user_name = getpwuid((long)fileStat.st_uid);

                                        printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
                                        printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
                                        printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
                                        printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
                                        printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
                                        printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
                                        printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
                                        printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
                                        printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
                                        printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");

                                        printf("  %lu", fileStat.st_nlink);

                                        printf(" %4s %4s ", user_name->pw_name, user_group->gr_name);

                                        printf("%lu", fileStat.st_size);

                                        char *foo = ctime(&fileStat.st_ctime);
                                        foo[strlen(foo) - 1] = 0;

                                        printf(" %s", foo);

                                        printf(" %s \n", myfile->d_name);

                                        // free(foo);
                                        // free(user_group);
                                        // free(user_name);
                                }
                        }

                        else
                        {
                                if (s - count > 2)
                                        printf("%s:\n", args[i]);
                                //char buf[512];
                                while ((myfile = readdir(mydir)) != NULL)
                                {
                                        if (flagA == 0)
                                        {
                                                if (myfile->d_name[0] == '.')
                                                        continue;
                                        }
                                        //  sprintf(buf, "%s/%s", argv[1], myfile->d_name);
                                        //  stat(buf, &fileStat);
                                        printf(" %s \n", myfile->d_name);
                                }
                        }
                } //end of for
        }

        closedir(mydir);
        // free(mydir);
        // free(myfile);
        return 0;
}


int run(char **args,int bg)
{
        if (args[0] == NULL)
                return 1;
      
        if (strcmp(args[0], "exit") == 0)
                return 0;
        int i;
        pid_t pid;
        int status, len = strstrlen(args);
        pid = fork();
        if (pid == 0)
        {
                //Child process
                if (strcmp(args[len - 1], "&") == 0)
                        args[len - 1] = NULL;
               
                if (execvp(args[0], args) == -1)
                        perror("Error");
                exit(EXIT_FAILURE);
        }
        else if (pid < 0)
                perror("Error forking");
        else
        {
                //Parent process
                if (strcmp(args[len - 1], "&") == 0)
                {
                        printf("Process ID of %s: %d\n", args[0], pid);
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
                else
                {
                        do
                        {
                                waitpid(pid, &status, WUNTRACED);
                        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
                }
        }
        return 1;
}

void doneProcesses()
{
        int i;
        for (i = 0; i < CHILDPROCESSESLEN; i++)
        {
                if (childProcesses[i] == -73)
                        continue;
                int cstatus = 10;
                waitpid(childProcesses[i], &cstatus, WNOHANG);
                // printf("%d %d\n", childProcesses[i], cstatus);
                if (cstatus != 10)
                {
                        fprintf(stderr, "%s with process ID %d exited normally\n", childNames[i], childProcesses[i]);
                        childProcesses[i] = -73;
                }
        }
}
/*
void handler(int sig)
{
  pid_t pid;
  pid = wait(NULL);
  if(pid!=-1)
    printf("\n[%d] done.\n", pid);
    
}

int run(char **args, int bgflag)
{
  int pid, wpid;
  int status;
  signal(SIGCHLD, handler);

  pid = fork();
  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror(RED " error while executing " RESET);
    }
    exit(1);
  } else if (pid < 0) {
    perror( RED "error while forking " RESET);
  }
  if (!bgflag) {
    waitpid(pid, &status, WUNTRACED);
  } else {
    do_in_background++;
    printf("[%d] %d\n", do_in_background, pid);
    waitpid(pid, &status, WNOHANG);
  }
return 1;
}
*/
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

int main(int argc , char* argv[])
{
    int ret;
    int i = 0;
    for(;i<7;i++)
    {
        childProcesses[i]= -73;
    }
    ret = shell_launch();
    return ret;
}   