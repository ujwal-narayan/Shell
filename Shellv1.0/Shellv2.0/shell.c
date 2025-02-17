/**************************************************Main shell loop **************************************************/

#include "headers.h"

int main()
{
        //Basic Setup
        shell = STDERR_FILENO;                         /* FD for stderr */

        num_jobs = 0;

        input_cmd_tokens = malloc((sizeof(char)*MAX_BUF_LEN)*MAX_BUF_LEN);
        output_cmd_tokens = malloc((sizeof(char)*MAX_BUF_LEN)*MAX_BUF_LEN);            /* Initialisations and allocations */

        if(isatty(shell)) {                                                         /* test whether a stderr refers to a terminal */
                while(tcgetpgrp(shell) != (shell_pgid = getpgrp()))                  /* if it does, send signal to make process \
                                                                                 group or executable same as process group of stderr */
                        kill(shell_pgid, SIGTTIN);                             /* SIGTTIN sets terminal input for background processes */
        }

        signal (SIGINT, SIG_IGN);                                    /* To ignore Ctrl c */

        signal (SIGTSTP, SIG_IGN);                                   /* To ignore Ctrl z */

        signal (SIGQUIT, SIG_IGN);                                   /* To ignore Ctrl \ */

        signal (SIGTTIN, SIG_IGN);                                   /* To ignore background processes */
        
        signal (SIGTTOU, SIG_IGN);

        my_pid = my_pgid = getpid();                                 /* Set pgid of executable same as pid */
        setpgid(my_pid, my_pgid);
        tcsetpgrp(shell, my_pgid);                                   /* Give control of stderr to executable's process group */
        if (getcwd(home_dir,MAX_BUF_LEN)==NULL)
        {
            perror(RED "Error while finding the current directory\n" RESET);
            return 1;
        }
        //Command Loop
        while(1) {
                if(signal(SIGCHLD,signal_handler)==SIG_ERR)
                        perror("can't catch SIGCHLD");
                if(signal(SIGINT,signal_handler)==SIG_ERR)
                        perror("can't catch SIGINT!");

                if (getcwd(curr_dir,MAX_BUF_LEN)==NULL)
        {
            perror(RED "Error while finding the current directory\n" RESET);
            return 1;
        }
        if (gethostname(hostname,MAX_BUF_LEN)==-1)
        {
            perror(RED "Error in finding hostname\n" RESET);
            return 1;
        }
        if (getlogin_r(user,MAX_BUF_LEN)!=0)
        {
            perror(RED "Error in finding the username\n" RESET);
            return 1;
        }
        if(strcmp(home_dir , curr_dir) == 0)
        {
            display[0] = home_sym;
            display[1] = '\0';
        }
        else
        {
            strcpy(display,curr_dir);
            display_home_converter(display,home_dir);
        }
        printf(CYAN "<" BLUE "%s" CYAN "@" YELLOW "%s" CYAN ":%s>" RESET,user,hostname,display);
                int i,j;

                char** cmds = malloc((sizeof(char)*MAX_BUF_LEN)*MAX_BUF_LEN); // array of semi-colon separated commands

                for(j = 0; j < MAX_BUF_LEN; j++) cmds[j] = '\0';

                char* cmdline = read_cmdline(); // read command line
                int num_cmds = parse_cmd_line(cmdline, cmds); // parse command line

                for(i = 0; i < num_cmds; i++) {
                        infile = outfile = NULL;
                        is_bg = 0, num_pipe = 0;
                        char* cmd_copy = strdup(cmds[i]);

                        char** cmd_tokens = malloc((sizeof(char)*MAX_BUF_LEN)*MAX_BUF_LEN); // array of command tokens
                        for(j = 0; j < MAX_BUF_LEN; j++) cmd_tokens[j] = '\0';

                        if(check_for_pipe(strdup(cmds[i])) == -1) {
                                if(input_redi == 1 || output_redi == 1) normal_cmd(parse_for_redi(strdup(cmd_copy), cmd_tokens), cmd_tokens, cmd_copy);
                                else {
                                        int tokens = parse_cmd(strdup(cmds[i]), cmd_tokens);
                                //        if(strcmp(cmd_tokens[0], "quit\0") == 0) exit(0);
                                        normal_cmd(tokens, cmd_tokens, cmd_copy);
                                }
                        }
                        else redi_and_pipi_cmd(cmds[i]);
                }
                if(cmds) free(cmds);
                if(cmdline) free(cmdline);
        }
        return 0;
}
