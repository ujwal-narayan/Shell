
#include "glob_var.h"


void doneProcesses()
{
        int i;
        for (i = 0; i < CHILDPROCESSESLEN; i++)
        {
                if (childProcesses[i] == -73)
                        continue;
                int cstatus = 10;
                waitpid(childProcesses[i], &cstatus, WNOHANG);
                // printf("%d %d\n", childProcesses[i], cstatus);
                if (cstatus != 10)
                {
                        fprintf(stderr, "%s with process ID %d exited normally\n", childNames[i], childProcesses[i]);
                        childProcesses[i] = -73;
                }
        }
}