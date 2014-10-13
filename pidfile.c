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
    int pid;
    
    //Check to see if pidfile already exists
    if (access ( PIDFILE, R_OK ) == 0)
    {   
        fprintf (stderr, "Error: PID file %s already exists\n", PIDFILE);
        fprintf (stderr, "Is pisound already running?\n");
        return 1;
    }
  
    //If not, make a pidfile
    fd = fopen (PIDFILE, "w");
    if (fd == NULL)
    {
        fprintf(stderr, "Error: Can't open or create %s: %s\n", PIDFILE, strerror(errno));
        return 1;
    }
   
    pid = getpid ();
    fprintf (fd, "%d\n", pid);
    fclose (fd);
    return 0;
}
