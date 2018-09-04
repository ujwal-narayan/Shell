#include "glob_var.h"
#include "cd.h"
#include "pwd.h"
#include "echo.h"
#include "pinfo.h"
#include "ls.h"
#include "remindee.h"
#include "clck.h"

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
    else if (strcmp(args[0],"exit")==0)
        exit(0);
    else if (strcmp(args[0],"remindme")==0)
        remindmee(args);
    else if (strcmp(args[0],"clock")==0)
        clck(args);
    else if (strcmp(args[0],"help")==0)
    {
        printf("Help yourself\n");
        return 0;
    }

    return -73;


}