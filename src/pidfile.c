#include "pisound.h"

#define PIDFILE "/tmp/pisound.pid"

int remove_pid (void)
{
    if (verbose)
        fprintf (stdout, "Removing PID file %s\n", PIDFILE);
    if (remove (PIDFILE) != 0)
    {
        fprintf (stderr, "Error removing PID file %s: %s\n", PIDFILE, strerror(errno));
        return 1;
    }
    else
        return 0;
}

int check_pid (void)
{
    FILE *fd;
    int pid = getpid ();
    int oldpid;
    
    //Check to see if pidfile already exists
    fd = fopen (PIDFILE, "r");
    if (fd != NULL)
    {  
        //TODO Why does this give me such a fucking weird number?
        //fread (&oldpid, sizeof(oldpid), 1, fd);
        
        //Read pid into oldpid
        fscanf (fd, "%d", &oldpid);
        
        if (verbose)
            fprintf (stdout, "PID file %s already exists, checking for process %d\n", PIDFILE, oldpid);
        
        //Check to see if the old process is still running
        if (pid != oldpid && kill (oldpid, 0) == 0)
        {
            fprintf (stderr, "pisound is already running as PID %d\n", oldpid);
            fclose(fd);
            return 1;
        }
        else
        {
            if (verbose)
                fprintf (stdout, "PID %d is no longer running, removing PID file and generating new one\n", oldpid);
            fclose(fd);
            remove (PIDFILE);
        }
    }

    //Lets make a new pidfile!
    fd = fopen (PIDFILE, "w");
    if (fd == NULL)
    {
        fprintf(stderr, "Error: Can't open or create %s: %s\n", PIDFILE, strerror(errno));
        return 1;
    }
   
    //Write PID to file
    fprintf (fd, "%d\n", pid);
    fclose (fd);
    return 0;
}
