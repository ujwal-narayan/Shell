#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>



int cd(char **args)
{
    return 0;
}

int pwd()
{
    return 0;
}

int echo(char **args)
{
    return 0;
}
int pinfo(char **args)
{
    return 0;
}


int ls(char **args)
{
    return 0;
}
int run(char **args,int do_in_background)
{
    return 0;
}
int execute_builtin(char **args)
{
    if(strcmp(args[0],"cd")==0)
    {
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
    if(args[0]=NULL)
    {
        return 1;
    }
    builtin = execute_builtin(args);
    if(builtin == -73)
       {
            return run(args,do_in_background);
       }
}