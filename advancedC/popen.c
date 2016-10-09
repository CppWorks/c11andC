#include <stdio.h>
#include <unistd.h>
int main()
{
    FILE* stream = popen("sort", "w"); // write to the chihld process so parent can write to the child
    // if is r, we read from the child process
    // The return value from popen is one end of a pipe; the other end is connected to the child process’s standard input

    // If the second
  //  argument is “w”, the function returns the child process’s standard input stream so that
    //    the parent can send data.If an error occurs, popen returns a null pointer.

    // so we are sending data....
    fprintf(stream, "This is a test.\n");
    fprintf(stream, "Hello, world.\n");
    fprintf(stream, "My dog has fleas.\n");
    fprintf(stream, "This program is great.\n");
    fprintf(stream, "One fish, two fish.\n");
    return pclose(stream);
}