#include<unistd.h> //Hostname 
#include <sys/utsname.h> //Username 
#include<stdio.h> //printf 
#include<string.h> // strotok 
#define MAX 1000000


//Global Variables 
char *rest , *R;

int string_compare(char a[] , char b[])
{
    
    for(int i = 0;a[i]!='\0';i++)
    {
        if(a[i]!=b[i])
            return 1;
    }
    return 0;
}
int parse_com(char cmd[])
{
    char *token ;
    char *rest = cmd ;
    while ((token = strtok_r(rest, " ", &rest)))
        printf("%s\n", token);

    return 0;
    
}
int main()
{
    char host_name[MAX],user_name[MAX], home_dir[MAX], curr_dir[MAX],cmd[MAX];
    int host_name_ret , user_name_ret ;
    char* home_dir_ret ;
    host_name_ret =  gethostname(host_name, MAX);
    user_name_ret =  getlogin_r(user_name,MAX);
    home_dir_ret = getcwd(home_dir,MAX);
   


    if (host_name_ret || user_name_ret || home_dir_ret == NULL)
    {
        perror("Error in hostname or username or homedir ");
        return 1;
    }
    while(1)
    {
        if(getcwd(curr_dir,MAX) ==NULL)
        {
            perror("Error in getting the current directort ");
            return 1;
        }


        if(string_compare(curr_dir,home_dir) == 0)
        {
            printf("<%s@%s:~>",user_name,host_name);
        }
        else 
        {  
            printf("<%s@%s:%s>",user_name,host_name,curr_dir);
        }
        if(fgets(cmd, sizeof(cmd), stdin) == NULL) 
        {
            perror("Error While taking input");
            return 1;
        }
        parse_com(cmd);
        
    }
    
    return 1;
}   
/*
 #include <stdio.h>
       #include <stdlib.h>
       #include <string.h>

       int
       main(int argc, char *argv[])
       {
           char *str1, *str2, *token, *subtoken;
           char *saveptr1, *saveptr2;
           int j;

           if (argc != 4) {
               fprintf(stderr, "Usage: %s string delim subdelim\n",
                       argv[0]);
               exit(EXIT_FAILURE);
           }

           for (j = 1, str1 = argv[1]; ; j++, str1 = NULL) {
               token = strtok_r(str1, argv[2], &saveptr1);
               if (token == NULL)
                   break;
               printf("%d: %s\n", j, token);

               for (str2 = token; ; str2 = NULL) {
                   subtoken = strtok_r(str2, argv[3], &saveptr2);
                   if (subtoken == NULL)
                       break;
                   printf(" --> %s\n", subtoken);
               }
           }

           exit(EXIT_SUCCESS);
       }
*/


