/*!
   \file amash.c
   \brief Contains implementations for core functions.
 */

#include "amash.h"
#include <string.h>

bool starts_with(const char *pre, const char *str)
{
    while(*pre != '\0')
        if(*(pre++) != *(str++))
            return false;
    return true;
}


char prompt[200];   /*!< A string containing the prompt to display */
/**
 *  Generate the prompt string, and copy it to `prompt`.
 */
void generatePrompt()
{
    char cwd[100];
    char login[100];
    char hostname[100];

    getcwd(cwd, sizeof(cwd));
    getlogin_r(login, sizeof(login));
    gethostname(hostname, sizeof(hostname));
    sprintf(prompt, "%s%s@%s%s:%s%s$%s ", KRED, hostname, KBLU, login, KYEL, cwd, KGRN);
}
/**
 *  Runs the main event loop
 *  @return return code
 */
int run_event_loop()
{
    char input[100];

    while (true)
    {
        generatePrompt();
        char* input = readline(prompt);
        log_debug("Read input '%s'", input);
        if(!input){
            do_quit(NULL);
        }

        add_history(input);

        ParsedInput *e = parse(input);
        exec(e);

        free(input);
    }
    return 0;
}

//TODO add something cool here

/**
 *  This function prints the intro splash screen at startup.
 */
void print_intro_screen()
{

}
/**
 *  The main function
 */
int main()
{
    print_intro_screen();
    log_set_level(LOG_ERROR);
    run_event_loop();
}
