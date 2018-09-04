#ifndef SHELL
#define SHELL

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
#include<wait.h>

#define home_sym '~'
#define RED "\033[0;31m"
#define RESET "\033[0m"
#define CHILDPROCESSESLEN 128

#define BUFF_SIZE 100 
pid_t childProcesses[CHILDPROCESSESLEN] = {};
char childNames[CHILDPROCESSESLEN][256] = {};
int run(char **args,int bg);

int strstrlen(char **arr)
{
    int i = 0;
    while(arr[i]!=NULL)
    {
        i++;
    }
    return i;
}

char home_dir[100],prev_dir[100],curr_dir[100];
int do_in_background = 0;

pid_t childProcesses[CHILDPROCESSESLEN] ;
char childNames[CHILDPROCESSESLEN][256] ;
#endif