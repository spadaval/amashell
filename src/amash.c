/*!
 * \file amash.c
 * \brief Contains implementations for core functions.
 */

#include "amash.h"
#include <string.h>


/**
 *  Runs the main event loop
 *  @return return cod
 */
int run_event_loop()
{
    while (true)
    {
        bool continued = false;
        generatePrompt();
        char* input = readline(prompt);

        log_debug("Read input '%s'", input);

        if (!input)
        {
            do_quit(NULL);
        }

        push_history(input);

        for (int i = 0; i < count_lines(input); i++)
        {
            char* s = extract_line(input);
            log_debug("Running input:%s", s);
            run_input(s);
        }
        // /run_input(input);
        //offset_sc = 0;

        //ParsedInput *e = parse(input);
        //exec(e);
    }
    return 0;
}


/// \todo add something cool here

/**
 *  This function prints the intro splash screen at startup.
 */
void initialize()
{
    aliases = malloc(sizeof(Alias));
    for (size_t i = 0; i < MAX_ALIASES; i++)
    {
        aliases->is_set[i]       = false;
        aliases->keys[i]         = NULL;
        aliases->values[i]       = NULL;
        aliases->insert_position = 0;
    }
    printf("Amash Shell v1.0\n");
}


/**
 *  The main function
 */
int main()
{
    initialize();
    //log_set_level(LOG_ERROR);
    run_event_loop();
}


void push_history(char* input)
{
    add_history(input);
}
