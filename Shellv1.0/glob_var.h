char home_dir[100],prev_dir[100],curr_dir[100];
int do_in_background = 0;
int BUFF_SIZE = 100; 

pid_t childProcesses[CHILDPROCESSESLEN] ;
char childNames[CHILDPROCESSESLEN][256] ;