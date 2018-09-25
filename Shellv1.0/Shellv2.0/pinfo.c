/********************************** For implementing pinfo command *****************************************/

#include "headers.h"

int pinfo(char **args)
{
        int c, L = 0;
        pid_t Id;

        

        if (args[1] == NULL)
                Id = getpid();

        else
                Id = atoi(args[1]);

        char Reuse_p[1024];
        char buffer[1024];

        printf("pid -- %d\n", Id);

        sprintf(Reuse_p, "/proc/%d/status", Id); //pass value

        FILE *fp = fopen(Reuse_p, "r");
        if (fp==NULL)
        {

                perror(RED "Error: could not open proc/pid/status file\n" RESET);
        }
        else
        {
                char status;
                do
                {
                        fgets(buffer, 256, fp);
                } while (buffer[0] != 'S');
                sscanf(buffer, "State:\t%c", &status);

                printf("Process Status -- %c\n", status);
                fclose(fp);
        }

        sprintf(Reuse_p, "/proc/%d/statm", Id);
        fp = fopen(Reuse_p, "r");
        if (fp==NULL)
        {

                perror(RED "Error: could not open proc/pid/statm file\n" RESET);
        }
        else
        {
                while ((c = fgetc(fp)) != ' ')
                        buffer[L++] = c;

                buffer[L] = '\0';

                printf("Memory -- %s\n", buffer);
                fclose(fp);
        }

        sprintf(Reuse_p, "/proc/%d/exe", Id);
        L = -4;
        L = readlink(Reuse_p, buffer,1023);
        if (L == -4)
        {

                perror(RED "Error: could not open proc/pid/exe file\n" RESET);
        }
        else
        {
                buffer[L] = '\0';       

                display_home_converter(buffer,home_dir);
                printf("Executable Path -- %s\n", buffer);
        }
        return 0;
    
}
