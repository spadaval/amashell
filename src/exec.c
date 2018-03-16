/*!
   \file exec.c
   \brief Contains subroutines for executing parsed input.
*/
#include "amash.h"

//TODO finish this
void set_redirects(ExecutableOptions* e){

}

void exec_program(ExecutableOptions* e){
    if(fork() == 0){
        /*child*/
        set_redirects(e);
        execlp(e->exec_path);
    }
}

void exec_input(ParsedInput* i){
    //TODO make this work for pipelined programs
    assert(i->exec_strings_count == 1);

    ExecutableOptions* e = malloc(sizeof(ExecutableOptions));
    e->exec_path = i->exec_paths[0];
    e->redirects = i->redirects;
    e->redirects_count = i->redirects_count;

    exec_program(e);
}
