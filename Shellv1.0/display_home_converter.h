int display_home_converter(char display[], char home_dir[])
{
    int size = strlen(display) , i , j=1;
    char temp[size+1];
    int is_parent_of_home = 0;

    if(size  < strlen(home_dir))
        return 0;

    for (i = 0 ; home_dir[i]!='\0' ;i++ )
    {
        
        if (display[i] == home_dir[i])
            continue;
        else 
        {
            is_parent_of_home = 1 ;
            break;

        }

    }
    if (is_parent_of_home == 1)
        return 0;
    temp[0]= '~';
    while (i<size)
    {
        temp[j] = display[i];
        j++;
        i++;
    }
    strcpy (display , temp );
    return 0;
    
}