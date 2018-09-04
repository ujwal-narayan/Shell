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
#include "global_var.h"
#include "shell_launch.h"

#define RED "\033[0;31m"
#define RESET "\033[0m"
#define CHILDPROCESSESLEN 128



 do_in_background = 0;
 BUFF_SIZE = 100; 

childProcesses[CHILDPROCESSESLEN] = {};
	childNames[CHILDPROCESSESLEN][256] = {};
int strstrlen(char **arr)
{
    int i = 0;
    while(arr[i]!=NULL)
    {
        i++;
    }
    return i;
}

// char home_dir[100],prev_dir[100],curr_dir[100];
// int do_in_background = 0;

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