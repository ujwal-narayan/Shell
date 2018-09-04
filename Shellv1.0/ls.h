#include "glob_var.h"
int ls(char **args)
{
        DIR *mydir;
        struct dirent *myfile;
        struct stat fileStat;
        int detect_long, detect_hidden, count;
        count = detect_long = detect_hidden = 0;
        int s = strstrlen(args);
        int i;
        for (i = 1; i < s; i++)
        {
                if (strcmp(args[i], "-l") == 0)
                {
                        detect_long = 1;
                        count++;
                }

                else if (strcmp(args[i], "-a") == 0)
                {
                        detect_hidden = 1;
                        count++;
                }

                else if (strcmp(args[i], "-la") == 0 )
                {
                        detect_hidden = 1;
                        detect_long = 1;
                        count++;
                }

                else if (strcmp(args[i], "-al") == 0)
                {
                        detect_hidden = 1;
                        detect_long = 1;
                        count++;
                }
        }

        if (s - count == 1)
        {
                mydir = opendir("./");
                if(mydir == NULL)
                {
                    perror( RED "Error in opening the directory\n" RESET);
                    return 0;
                }
                if (detect_long == 1)
                {
                       
                        char buf[512];
                        while ((myfile = readdir(mydir)) != NULL)
                        {
                                if (detect_hidden == 0)
                                {
                                        if (myfile->d_name[0] == '.')
                                                continue;
                                }
                                int ret = sprintf(buf, "%s/%s", "./", myfile->d_name);
                                if(ret < 0 )
                                {
                                    perror(RED "Error while printing\n" RESET);
                                    return 1;
                                }
                                stat(buf, &fileStat);

                                struct group *user_group = getgrgid((long)fileStat.st_gid);
                                struct passwd *user_name = getpwuid((long)fileStat.st_uid);

                                

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



                                printf("  %lu", fileStat.st_nlink);

                                printf(" %4s %4s ", user_name->pw_name, user_group->gr_name);

                                printf("%lu", fileStat.st_size);

                                char *varia = ctime(&fileStat.st_ctime);
                                varia[strlen(varia) - 1] = 0;

                                printf(" %s", varia);

                                printf(" %s \n", myfile->d_name);

                                
                        }
                }

                else
                {
                       
                        while ((myfile = readdir(mydir)) != NULL)
                        {
                                if (detect_hidden == 0)
                                {
                                        if (myfile->d_name[0] == '.')
                                                continue;
                                }
                                
                                printf(" %s \n", myfile->d_name);
                        }
                }
        }

        else
        {
                int i = 1;
                while(i<s)
                {
                       
                        if (args[i][0] == '-')
                        {
                              
                                printf("This is not supported as a builtin command. Do you still want to run it (y?)");
                                char y ;
                                scanf("%c",&y);
                                if(y == 'y')
                                    run(args,0);
                                else
                                    return 0;
                        }
                      
                        char tmp[512] = "";
                        if (args[i][0] == home_sym )
                        {
                                const struct passwd *pw = getpwuid(getuid());
                                const char *HOME = pw->pw_dir;
                                strcat(tmp, HOME);
                                strcat(tmp, &args[i][1]);
                                mydir = opendir(tmp);
                                if(mydir == NULL)
                                {
                                    perror( RED "Error in opening the directory\n" RESET);
                                    return 0;
                                }
                                
                        }
                        else
                                mydir = opendir(args[i]);
                                if(mydir == NULL)
                                {
                                    perror( RED "Error in opening the directory.\n" RESET);
                                    return 0;
                                }

                       

                        if (detect_long == 1)
                        {
                                if (s - count > 2)
                                        printf("%s:\n", args[i]);
                                char buf[512];
                                while ((myfile = readdir(mydir)) != NULL)
                                {
                                        if (detect_hidden == 0)
                                        {
                                                if (myfile->d_name[0] == '.')
                                                        continue;
                                        }
                                        if (args[i][0] == home_sym )
                                                sprintf(buf, "%s/%s", tmp, myfile->d_name);
                                        else
                                                sprintf(buf, "%s/%s", args[i], myfile->d_name);
                                        stat(buf, &fileStat);

                                        struct group *user_group = getgrgid((long)fileStat.st_gid);
                                        struct passwd *user_name = getpwuid((long)fileStat.st_uid);


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


                                        printf("  %lu", fileStat.st_nlink);

                                        printf(" %4s %4s ", user_name->pw_name, user_group->gr_name);

                                        printf("%lu", fileStat.st_size);

                                        char *varia = ctime(&fileStat.st_ctime);
                                        i++;
                                        varia[strlen(varia) - 1] = 0;

                                        printf(" %s", varia);

                                        printf(" %s \n", myfile->d_name);

                                        
                                }
                        }

                        else
                        {
                                if (s - count > 2)
                                        printf("%s:\n", args[i]);
                                while ((myfile = readdir(mydir)) != NULL)
                                {
                                        if (detect_hidden == 0)
                                        {
                                                if (myfile->d_name[0] == '.')
                                                        continue;
                                        }
                                        printf(" %s \n", myfile->d_name);
                                }
                        }
                } //end of for
        }

        closedir(mydir);
        return 0;
}