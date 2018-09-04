#include "glob_var.h"
int remindMe(char *timer , char* mes)
{
    int time = atoi(timer);
    int pid = fork();
    if(pid == 0)
    {
        sleep(time);
        printf("\nReminder: %s \n", mes);
    }
    return 0;
}

int remindmee(char **args)
{
    remindMe(args[1],args[2]);return 0;
}