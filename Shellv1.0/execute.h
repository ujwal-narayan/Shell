int execute(char **args , int do_in_background)
{
    int builtin;
    
    if(args[0]==NULL)
    {
       // printf("Come on , plis work\n");
        return 1;
    }
    //printf("HI ,args : %s\n",args[0]);;
    builtin = execute_builtin(args);
    if(builtin == -73)
       {
            return run(args,do_in_background);
       }
    else 
        return builtin;
}