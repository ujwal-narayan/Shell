#include<unistd.h> //Hostname 
#include <sys/utsname.h> //Username 
#include<stdio.h>
#define MAX 1000000
int string_compare(char a[] , char b[])
{
    
    for(int i = 0;a[i]!='\0';i++)
    {
        if(a[i]!=b[i])
            return 1;
    }
    return 0;
}
int main()
{
    char host_name[MAX],user_name[MAX], home_dir[MAX], curr_dir[MAX];
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
    if(getcwd(curr_dir,MAX) ==NULL)
    {
        perror("Error in getting the current directort ");
        return 1;
    }


    if(string_compare(curr_dir,home_dir) == 0)
    {
        printf("<%s@%s:~>\n",user_name,host_name);
        return 0;
    }
    else 
    {  
        printf("<%s@%s:%s>\n",user_name,host_name,curr_dir);
        return 0;
    }

    
    return 1;
}   
