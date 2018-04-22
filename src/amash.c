/*!
 * \file amash.c
 * \brief Contains implementations for core functions.
 */

#include "amash.h"
#include <string.h>


/**
 *  Runs the main event loop
 *  @return return code
 */
int run_event_loop()
{
        while (true)
        {
                printf("\n");
                generatePrompt();
                char* input = readline(prompt);

                log_debug("Read input '%s'", input);

                if (strlen(input)==0)
                {
                        do_quit(NULL);
                }

                push_history(input);

                for (int i = 0; i < count_lines(input); i++)
                {
                        char* s = extract_line(input);
                        if(offset_sc==0)
                        {
                                break;
                        }
                        log_debug("Running input:%s", s);
                        run_input(s);
                }
                //run_input(input);
                offset_sc = 0;

                //ParsedInput *e = parse(input);
                //exec(e);
        }
        return 0;
}


/// TODO add something cool here

/**
 *  Sets up initial data structures and prints the splash screen/MOTD at startup
 */
void initialize()
{
        log_set_level(LOG_TRACE);
        aliases = new_pairlist();
        int pid;
        if ((pid = vfork()) == 0)
        {
                printf("\n\n\n");
                execle("/usr/bin/figlet\0","figlet\0","Amash Shell v1.1\0",NULL);
                sleep(1);
                //system("figlet \"Amash Shell v1.1\"");
                exit(0);
        }
        wait(NULL);
}


/**
 *  The main function
 */
int main()
{
        initialize();
        run_event_loop();
}


void push_history(char* input)
{
        add_history(input);
}
