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
//TODO finish this
void exec_input(ParsedInput* i){

}
