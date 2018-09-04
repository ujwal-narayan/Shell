#include "glob_var.h"

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
