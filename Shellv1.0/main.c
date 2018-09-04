
#include "glob_var.h"
#include "shell_launch.h"





// do_in_background = 0;
//  BUFF_SIZE = 100; 

// childProcesses[CHILDPROCESSESLEN] = {};
// childNames[CHILDPROCESSESLEN][256] = {};


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