/*****************************Header Files & Macros*****************************************/
#ifndef SHELL

#define SHELL

#include<signal.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<sys/types.h>
#include <sys/types.h>
 #include <sys/wait.h>
#define MAX_BUF_LEN 1024
#define append_str ">>"
#define CMD_DELIMS " \t\n"
#define home_sym '~'
#define pipe_sym '|'
#define input_redi_sym '<'
#define output_redi_sym '>'
#define input_redi_str "<"
#define output_redi_str ">"
#define pipe_str "|"
#define RED "\033[0;31m"
#define BLUE "\033[0;34m"
#define YELLOW "\033[0;33m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"
#define FNAME_LEN 20

/**********************************Function Declarations************************************/

void check_bg_finish();
void add_proc(int pid, char* name);
void rem_proc(int pid);
char* read_cmdline();
int parse_cmd_line(char* cmd, char** cmds);
int parse_cmd(char* cmd, char** cmd_tokens);
void set_prompt();
void get_home_dir();
int run_cmd(char** cmd_tokens);
void mod_curr_dir_rel(char* curr_dir);
int cd_cmd(char** cmd_tokens, char* curr_dir, char* home_dir);
int run_bg_cmd(char** cmd_tokens, int tokens);
int echo(char* line);
void pwd(char** cmd_tokens);
void get_prompt_vars();
int pinfo(char** cmd_tokens);
int open_infile();
int open_outfile();
void normal_cmd(int tokens, char** cmd_tokens, char* copy);
int check_for_pipe(char* cmd);
void redi_and_pipi_cmd(char* cmd);
int kjob(char** cmd_tokens);
void overkill();
void initializer();
void signal_handler(int signum);
int fg(char** cmd_tokens);
void normal_cmd(int tokens, char** cmd_tokens, char* cmd_copy);
void redi_and_pipi_cmd(char* cmd);

/*******************************Variable And Struct Declarations******************************/

struct proc_fields {
        int pid, pgid;
        char* name;
        int active;
};

typedef struct proc_fields proc_fields;

int num_jobs;

proc_fields table[MAX_BUF_LEN];

char user[MAX_BUF_LEN];
char hostname[MAX_BUF_LEN];
char home_dir[MAX_BUF_LEN];
char display[MAX_BUF_LEN];
char prev_dir[MAX_BUF_LEN];
char* pipe_cmds[MAX_BUF_LEN];
char curr_dir[MAX_BUF_LEN]; /* current working directory */

char* infile;
char* outfile;

int shell, shell_pgid;

int last, num_pipe;

int piping, input_redi, output_redi;

char** input_cmd_tokens;
char** output_cmd_tokens;
char** input_redi_cmd;
char** output_redi_cmd;

pid_t my_pid, my_pgid, fgpid;

int is_bg, idxi, idxo;

#endif
