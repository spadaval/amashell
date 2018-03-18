/*!
   \file exec.c
   \brief Contains subroutines for executing parsed input.
*/
#include "amash.h"
#include <sys/wait.h>
//TODO finish this
void set_redirects(Executable* e)
{

}

void exec_program(Executable* e)
{
        printf("\n\tPath : %s",e->exec_path);
        printf("\n\tArguments : ");
        int i;
        for(i=0; i<e->argc; i++)
        {
                printf("%s ",e->argv[i]);
        }
        if(fork() == 0)
        {
                /*child*/
                printf("\n\tFork Successful");
                //set_redirects(e);
                execvp(e->exec_path, e->argv);
        }
        wait(NULL);
        printf("\n\nProcess Returned\n");
}
/*
   void exec_input(ParsedInput* i)
   {
        assert(false);
        //TODO make this work for pipelined programs
        assert(i->exec_strings_count == 1);
   }
 */


void dump_executable(Executable* e){
    log_debug("Beginning dump:");
    log_debug("\tExec path: %s", e->exec_path);
    log_debug("\tNumber of redirects: %d", e->redirects_count);
    for(int i = 0; i < e->argc; i++)
        log_debug("Argument %d: %s", i, e->argv[i]);
}
