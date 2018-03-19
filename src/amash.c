/*!
   \file amash.c
   \brief Contains implementations for core functions.
*/

#include "amash.h"
#include <string.h>

void strip_input(char * input)
{
        input[strlen(input)-1] = '\0';
}

bool startsWith(const char *pre, const char *str)
{
        return strncmp(pre, str, strlen(pre)) == 0;
}

void printPrompt()
{
    char cwd[100];
    char login[100];
    char hostname[100];
    getcwd(cwd, sizeof(cwd));
    getlogin_r(login, sizeof(login));
    gethostname(hostname, sizeof(hostname));
    printf(KBLU "\n%s@%s",hostname,login);
    printf(KRED":");
    printf(KWHT"%s",cwd);
    printf(KRED"$");
    printf(KGRN "");
}

int run_event_loop()
{
    char input[100];
        while(1)
        {
                printPrompt();
                fgets(input, 1000, stdin);
                strip_input(input);

                log_debug("input : <%s>",input);

                Executable* e = parse_single(input);
                exec_program(e);

        }
        return 0;
}

//TODO add something cool here
/*
void print_intro_screen()
{

}
*/

int main()
{
        //print_intro_screen();
        run_event_loop();
}
