#include "global_var.h"
char* get_input()
{
    char* line = NULL ;
    size_t len ;
    ssize_t read;
    read = getline(&line,&len,stdin);
    if(read == -1)
    {
        perror(RED "Error while taking the input \n" RESET);
        return NULL;
    }
    else
    {
        return line;

    }

}


if((S_ISDIR(fileStat.st_mode)))
                                    printf("d");
                                else
                                    printf("-");
                                if (fileStat.st_mode & S_IRUSR)
                                    printf("r");
                                else
                                    printf("-");
                                if(fileStat.st_mode & S_IWUSR)
                                    printf("w");
                                else
                                    printf("-");
                                if(fileStat.st_mode & S_IXUSR)
                                    printf("x");
                                else
                                    printf("-");
                                if(fileStat.st_mode & S_IRGRP)
                                    printf("r");
                                else
                                    printf("-");
                                if (fileStat.st_mode & S_IWGRP)
                                    printf("w");
                                else
                                    printf("-");
                                if (fileStat.st_mode & S_IXGRP)
                                    printf("x");
                                else
                                    printf("-");
                                if (fileStat.st_mode & S_IROTH)
                                    printf("r");
                                else
                                    printf("-");
                                if (fileStat.st_mode & S_IWOTH)
                                    printf("w");
                                else
                                    printf("-");
                                if(fileStat.st_mode & S_IXOTH)
                                    printf("x");
                                else
                                    printf("-");