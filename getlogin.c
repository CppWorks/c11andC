#include<stdio.h>

#include<string.h>

// if either tty_name_max or login_name_max are undefined, this means they are indeterminate values, assume a resoable size for simplicity rather than using sysconf and dynamically allocaitng the buffers

#ifndef TTY_NAME_MAX
#define TTY_NAME_MAX 120
#endif

#ifndef LOGIN_NAME_MAX
#define LOGIN_NAME_MAX 32
#endif


int main (int argc, char *argv[])
{
 
    char UserName[] = "oddo";
    printf("Please enter your user name: \n");
    scanf("%s", UserName);
    
    printf("Please enter your password: \n");
    char PassWord[] = "77";
    scanf("%s", PassWord);
    //if...else statement to test if the input is the correct username. 
    if (UserName == "oddo") 
    {
        printf("user is %s", UserName);
        if (PassWord == "Chocolate"){
            printf("Welcome!\n");
        }
    }else
    {
        printf("The user name or password you entered is invalid.\n");
    }

    //char login_str[LOGIN_NAME_MAX];
    //char stdin_str[TTY_NAME_MAX];
    //char cterm_str[L_ctermid], *cterm_str_ptr;
    //int status;
    //status = getlogin_r(login_str, sizeof(login_str));
    //if (status != 0)
    //    err_abort(status, "Get login");
    ////cterm_str_ptr = ctermid(cterm_str);
    ////if (cterm_str_ptr == NULL)
    ////     err_abort(0, "Get ceterm");
    //status = ttyname_r(0, stdin_str, sizeof(stdin_str));
    //if (status != 0)
    //    err_abort(status, "Get stdin");
    //printf(stdin_str);
    //printf("User: %s, cterm: %s, fd 0: %s\n", login_str, cterm_str, stdin_str);
    return 0;

}
// gcc -Wno-implicit alarm.c -o hello -lpthread

