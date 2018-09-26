
#include "header.h"


int open_outfile() {
        int files;
        if(last == 2) files = open(outfile, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
        if(last == 1) files = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
        if(files < 0) {
                perror( RED "Error with output" RESET);
        }
        dup2(files, STDOUT_FILENO);
        close(files);
        return files;
}
int open_infile() {
        int files = open(infile, O_RDONLY, S_IRWXU);
        if (files < 0) {
                perror(RED "Error with input" RESET);
                
        }
        dup2(files, STDIN_FILENO);
        close(files);
        return files;
}
