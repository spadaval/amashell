/*!
   \file amash.h
   \brief Contains all the major struct, union, and API function definitions.
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

int run_event_loop()
{
        char input[1000];
        while(1)
        {
                printf("\n$ ");
                fgets(input, 1000, stdin);

                strip_input(input);

                //printf("\ninput : %s END",input);
                if(startsWith("quit",input))
                {
                        break;
                }
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
