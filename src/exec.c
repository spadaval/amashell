/*!
   \file exec.c
   \brief Contains subroutines for executing parsed input.
 */
#include "amash.h"
#include <sys/wait.h>

void set_redirects(Executable *e)
{
    log_debug("Applying redirects");
    if (e->stdin != NULL)
    {
        int fd = open(e->stdin, O_RDONLY);
        dup2(STDIN_FILENO, fd);
        log_trace("Redirecting stdin to '%s'", e->stdin);
    }
    if (e->stdout != NULL)
    {
        int fd = open(e->stdout, O_WRONLY);
        dup2(STDOUT_FILENO, fd);
        log_trace("Redirecting stdin to '%s'", e->stdin);
    }
}

bool handle_builtins(Executable *e)
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
    {
        return false;
    }
}

void exec_program(Executable *e)
{
    if (handle_builtins(e))
    {
        log_trace("Processed as builtin");
    }
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
                log_debug("exec_path:'%s'(%d)", e->exec_path, strlen(e->exec_path));
                printf(KNRM "\namash: command not found : '%s'\n\n", e->exec_path);
                abort();
            }
        }
        wait(NULL);
    }
}

/// \todo implement the exec function
void exec(ParsedInput *input)
{
    int pipe_read_fd = -1, pipe_write_fd = -1;
    int fd[2];
    for(int i = 0; i < input->executables_count; i++){
        // set current executable as a local convenience variable
        Executable* e = &input->executables[i];
        if(i != input->executables_count - 1){
            //set read head to the previously opened pipe
            pipe_read_fd = fd[0];
            //open a new pipe to send data to the next executable
            pipe(fd);
            //set write head to the currently opened pipe
            pipe_write_fd = fd[1];
        }

        if (handle_builtins(e))
        {
            log_trace("Processed as builtin");
        }
        else
        {
            // fork and handle
            log_trace("\tBeginning forked process");

            if (fork() == 0)
            {
                /*child*/

                set_redirects(e);
                //if not the first executable
                if(i != 0) {
                    dup2(pipe_read_fd, fileno(stdin));
                }
                // if not the last executable
                if(i != input->executables_count - 1){
                    dup2(pipe_write_fd, fileno(stdout));
                }

                dump_executable(e);
                if (execvp(e->exec_path, e->argv) != 0)
                {
                    log_error(KRED "Exec error(error %d)", errno);
                    log_debug("exec_path:'%s'(%d)", e->exec_path, strlen(e->exec_path));
                    printf(KRED "\namash: command not found : '%s'\n\n", e->exec_path);
                    abort();
                }
            }
            wait(NULL);
        }

    }
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
