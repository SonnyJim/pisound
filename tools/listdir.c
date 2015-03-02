#include <stdio.h>
#include <dirent.h>
#include <string.h>

int main (int argc, char *argv[])
{
    DIR *dir;
    struct dirent *dp;
    char * file_name;
    char dir_name[64];
    int counter = 0;

    if (argc != 2)
    {
        fprintf (stderr, "Error:  Must supply a directory\n");
        fprintf (stderr, "Usage: listdir directory\n");
        return 1;
    }


    strcpy (dir_name, argv[1]);

    dir = opendir(argv[1]);
    if (dir == NULL)
    {
        fprintf (stderr, "Error opening directory %s\n", argv[1]);
        return 1;
    }

    if ( argv[1][strlen(argv[1]) -1 ] != '/')
    {
        strcat (dir_name, "/");
    }

    while ((dp=readdir(dir)) != NULL) {
        if ( !strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") )
        {
            // do nothing (straight logic)
        } 
        else 
        {
            file_name = dp->d_name; // use it
            printf("%i=%s%s\n",counter, dir_name, file_name);
            counter++;
        }
    }
    closedir(dir);
    return 0;
}
