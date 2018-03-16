/*!
   \file exec.c
   \brief Contains subroutines for executing parsed input.
*/
#include "amash.h"

//TODO finish this
void set_redirects(Executable* e){

}

void exec_program(Executable* e){
    if(fork() == 0){
        /*child*/
        set_redirects(e);
        execvp(e->exec_path, e->argv);
    }
}
/*
void exec_input(ParsedInput* i){
    assert(false);
    //TODO make this work for pipelined programs
    assert(i->exec_strings_count == 1);

}
*/
