#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main ()
{
    int fds[2];
    pid_t pid;
    /* Create a pipe. File descriptors for the two ends of the pipe are
    placed in fds. */
    pipe (fds);
    /* Fork a child process. */
    pid = fork ();
    if (pid == (pid_t) 0) {
        /* This is the child process. Close our copy of the write end of
        the file descriptor. */
        close (fds[1]);
        /* Connect the read end of the pipe to standard input. */
        dup2 (fds[0], STDIN_FILENO);
        /* Replace the child process with the “sort” program. */
        execlp ("sort", "sort", 0);
        /*
        This prototype:

        int execlp(const char *file, const char *arg, ...);
        Says that execlp ìs a variable argument function. 
        It takes 2 const char *. The rest of the arguments, 
        if any, are the additional arguments to hand over to program we want to run - also char * -
        all these are C strings (and the last argument must be a NULL pointer)

        So, the file argument is the path name of an executable file to be executed. 
        arg is the string we want to appear as argv[0] in the executable. By convention, 
        argv[0] is just the file name of the executable, normally it's set to the same as file.
        /*
    }
    else {
        /* This is the parent process. */
        FILE* stream;
        /* Close our copy of the read end of the file descriptor. */
        close (fds[0]);
        /* Convert the write file descriptor to a FILE object, and write
        to it. */
        stream = fdopen (fds[1], "w");
        fprintf (stream, "This is a test.\n");
        fprintf (stream, "Hello, world.\n");
        fprintf (stream, "My dog has fleas.\n");
        fprintf (stream, "This program is great.\n");
        fprintf (stream, "One fish, two fish.\n");
        fflush (stream);
        close (fds[1]);
        /* Wait for the child process to finish. */
        waitpid (pid, NULL, 0);
    }
    return 0;
}