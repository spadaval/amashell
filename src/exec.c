/*!
   \file exec.c
   \brief Contains subroutines for executing parsed input.
 */
#include "amash.h"
#include <sys/wait.h>
//TODO finish this
void set_redirects(Executable *e)
{

}

bool handle_builtins(Executable* e)
{
    if (startsWith("quit", e->exec_path))
    {
        log_trace("Quit detected");
        do_quit(e);
        return true;
    }
    else if (strcmp(e->exec_path, "cd") == 0)
    {
        do_cd(e);
        return true;
    }
    else if (strcmp(e->exec_path, "pwd") == 0)
    {
        do_pwd(e);
        return true;
    }
    else
        return false;

}

void exec_program(Executable *e)
{
    dump_executable(e);

    if(handle_builtins(e))
        log_trace("Processed as builtin");
    else
    {
        // fork and handle
        log_trace("\tBegin forked process");
        printf(KMAG "\n__________________________________________________________________________________________________________________________\n");
        printf(KCYN "\n");

        if (fork() == 0)
        {
            /*child*/
            set_redirects(e);
            if (execvp(e->exec_path, e->argv) != 0)
            {
                log_error("Exec error(error %d)", errno);
                printf(KNRM "\namash: command not found : '%s'\n\n", e->exec_path);
                abort();
            }
        }
        wait(NULL);
    }
}
/*
   void exec_input(ParsedInput* i)
   {
        assert(false);
        //TODO make this work for pipelined programs
        assert(i->exec_strings_count == 1);
   }
 */

void do_cd(Executable *e)
{
    if (e->argv[1] == NULL)
    {
        chdir("/");
    }
    else
    {
        chdir(e->argv[1]);
    }
}

void do_pwd(Executable *e)
{
    printf("%s\n", get_current_dir_name());
}

void do_quit(Executable *e)
{
    printf("\nSayonara!");
    exit(0);
}
