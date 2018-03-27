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
    if (starts_with("quit", e->exec_path))
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
    if(handle_builtins(e))
        log_trace("Processed as builtin");
    else
    {
        // fork and handle
        log_trace("\tBegin forked process");

        if (fork() == 0)
        {
            /*child*/
            set_redirects(e);

            dump_executable(e);
            if (execvp(e->exec_path, e->argv) != 0)
            {
                log_error("Exec error(error %d)", errno);
                log_debug("exec_path:'%s'(%d)", e->exec_path,strlen(e->exec_path));
                printf(KNRM "\namash: command not found : '%s'\n\n", e->exec_path);
                abort();
            }
        }
        wait(NULL);
    }
}
//TODO implement this
void exec(ParsedInput* i)
{
    assert(false);
    assert(i->executables_count == 1);
}


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
