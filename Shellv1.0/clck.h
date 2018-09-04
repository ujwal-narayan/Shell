#include "glob_var.h"
int clck(char **args)
{
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
    char ** asd= tokenizer(fd,2);
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
