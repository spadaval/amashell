/*!
 * \file exec.c
 * \brief Contains subroutines for executing parsed input.
 */
#include "amash.h"
#include <sys/wait.h>
#include <unistd.h>
#include <termios.h>


static struct termios old, new;
char *hist[HISTORY_COUNT];
int current;

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
        tcgetattr(0, &old); /* grab old terminal i/o settings */
        new = old; /* make new settings same as old settings */
        new.c_lflag &= ~ICANON; /* disable buffered i/o */
        if (echo)
        {
                new.c_lflag |= ECHO; /* set echo mode */
        }
        else
        {
                new.c_lflag &= ~ECHO; /* set no echo mode */
        }
        tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
        tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
        char ch;
        initTermios(echo);
        ch = getchar();
        resetTermios();
        return ch;
}

/* Read 1 character without echo */
char getch(void)
{
        return getch_(0);
}

/* Read 1 character with echo */
char getche(void)
{
        return getch_(1);
}


void set_redirects(Executable* e)
{
        log_trace("Applying redirects");

        if ((e->stdin != NULL) && (strcmp(e->stdin, "") != 0))
        {
                int fd = open(e->stdin, O_RDONLY);
                dup2(fd, STDIN_FILENO);
                log_trace("Redirecting stdin to '%s'", e->stdin);
        }
        else
        {
                log_trace("Not applying stdin redirect");
        }
        if ((e->stdout != NULL) && (strcmp(e->stdin, "") != 0))
        {
                int fd = open(e->stdout, O_WRONLY | O_CREAT);
                dup2(fd, STDOUT_FILENO);
                log_trace("Redirecting stdout to '%s'", e->stdout);
        }
        else
        {
                log_trace("Not applying stdout redirect");
        }
}


bool handle_builtins(Executable* e)
{
        int i;
        hist[current]=NULL;
        hist[current] = strdup(e->exec_path);
        current = (current + 1) % HISTORY_COUNT;

        if (starts_with("quit", e->exec_path) || starts_with("exit", e->exec_path))
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

        else if (strcmp(e->exec_path, "history") == 0)
        {
                do_history(hist, current);
                return true;
        }
        else if (strcmp(e->exec_path, "alias") == 0)
        {
                do_alias(e);
                return true;
        }
        else if (strcmp(e->exec_path, "env") == 0)
        {
                do_env(e);
                return true;
        }
        else if (strcmp(e->exec_path, "export") == 0)
        {
                do_export(e);
                return true;
        }
        else if (strcmp(e->exec_path, "ed2") == 0)
        {
                do_ed2(e);
                return true;
        }
        else if (strcmp(e->exec_path, "lock") == 0)
        {
                do_lock(e);
                return true;
        }
        else if ((strcmp(e->exec_path, "source") == 0) || (strcmp(e->exec_path, ".") == 0))
        {
                do_source(e);
                return true;
        }
        else if(strcmp(e->exec_path,"hclear")==0)
        {
                do_hclear(hist);
                return true;
        }
        else
        {
                return false;
        }
}


/// \todo implement the exec functiondump_executable
void exec(ParsedInput* input)
{
        log_trace("Begin exec");
        assert(input->executables_count > 0);

        int fd[2];

        for (int i = input->executables_count - 1; i >= 0; i--)
        {
                // set current executable as a local convenience variable
                Executable* e = &input->executables[i];
                dump_executable(e);

                if (handle_builtins(e))
                {
                        log_trace("Processed as builtin");
                }
                else
                {
                        // fork and handle

                        int read_target, write_target;

                        if (i != input->executables_count - 1)
                        {
                                write_target = fd[1];
                        }
                        if (i != 0)
                        {
                                pipe(fd);
                                log_trace("Create new pipe(fd[0]=%d, fd[1]=%d)", fd[0], fd[1]);
                                read_target = fd[0];
                        }

                        if (fork() == 0)
                        {
//                sleep(1);
                                /*child*/
                                set_redirects(e);


                                if (i != input->executables_count - 1)
                                {
                                        log_trace("Set %s output to write target %d", input->executables[i].exec_path, write_target);
                                        dup2(write_target, fileno(stdout));
                                }
                                if (i != 0)
                                {
                                        log_trace("Set %s input to read target %d", input->executables[i].exec_path, read_target);
                                        dup2(read_target, fileno(stdin));
                                }


                                log_trace("Setup complete, running exec:");
                                if (execvp(e->exec_path, e->argv) != 0)
                                {
                                        log_error(KRED "Exec error(error %d)", errno);
                                        log_debug("exec_path:'%s'(length %d)", e->exec_path, strlen(e->exec_path));

                                        printf(KRED "\namash: command not found : '%s'\n\n", e->exec_path);
                                        abort();
                                }
                        }
                }
        }

        wait(NULL);
}


void do_cd(Executable* e)
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


void do_pwd(Executable* e)
{
        printf("%s\n", get_current_dir_name());
}


void do_quit(Executable* e)
{
        printf("\nSayonara!");
        exit(0);
}


void do_history(char *hist[],int current)
{
        int i = current;
        int hist_num = 1;
        do {
                if (hist[i])
                {
                        printf("%4d  %s\n", hist_num, hist[i]);
                        hist_num++;
                }
                i = (i + 1) % HISTORY_COUNT;
        } while (i != current);

}

void do_hclear(char *hist[])
{
        int i;
        for (i = 0; i < HISTORY_COUNT; i++)
        {
                free(hist[i]);
                hist[i] = NULL;
        }
}

int run_input(char* input)
{
        input = resolve_input(input);
        ParsedInput* p = parse(input);

        exec(p);
        return 0;
}


void do_alias(Executable* e)
{
        if (e->argc == 1)
        {
                log_trace("Run do_alias in dump mode");
                for (int i = 0; i < MAX_PAIRS; i++)
                {
                        if (aliases->is_set[i])
                        {
                                printf("\n(%d)%s=%s", i, aliases->keys[i], aliases->values[i]);
                                printf("\n");
                        }
                }
        }
        else
        {
                log_trace("Run do_alias in set mode");

                assert(e->argc == 2);
                char* string = e->argv[1];
                log_trace("Got string:'%s'", string);
                char* key = calloc(sizeof(char), strlen(string));
                char* val = calloc(sizeof(char), strlen(string));
                char* k   = key;
                while (string && ((*string) != '='))
                {
                        *(k++) = *(string++);
                }
                log_trace("Got key:'%s', next character:'%c'", key, *string);
                string++;
                char* v = val;
                while (string && *string != ' ' && *string != '\0' && *string != '\n')
                {
                        *(v++) = *(string++);
                }
                log_trace("Got value '%s'", val);

                set_alias(key, val);
        }
}


void do_env(Executable* e)
{
        char** s = environ;

        while (*s != NULL)
        {
                printf("\n%s", *s++);
        }
}


void do_export(Executable* e)
{
        assert(e->argc == 2);
        putenv(e->argv[1]);
}


void do_source(Executable* e)
{
        log_trace("Running source on filepath '%s'", e->argv[1]);
        if (e->argc != 2)
        {
                printf("\nUsage: source /path/to/file\n");
                return;
        }

        FILE* f = fopen(e->argv[1], "r");
        if (!f)
        {
                printf("amash: unable to open file '%s'", e->argv[1]);
                return;
        }
        char* c = calloc(sizeof(char), INPUT_LENGTH);
        while (!feof(f))
        {
                fgets(c, INPUT_LENGTH, f);
                int i;
                offset_sc = 0;
                for (i = 0; i < count_lines(c); i++)
                {
                        char* s = extract_line(c);
                        log_debug("Running input:%s", s);
                        run_input(s);
                }
                offset_sc = 0;
        }
        fclose(f);
}

void do_ed2(Executable* e)
{
        int pid;
        if ((pid = vfork()) == 0)
        {
                execl("./../ed2/ed2\0","./ed2",NULL);
                exit(0);
        }
}

void do_lock(Executable* e)
{
        clear();
        while(1)
        {
                char new[40];
                int p;
                printf("\n\n\n\tUnlock : ");
                for(p=0; p<5; p++)
                {
                        new[p]=getch();
                        printf("*");
                }
                fflush(stdout);
                printf("\n");
                clear();
                sleep(1);
                //char* input = readline(new);
                if(strcmp(new,"admin")==0)
                {
                        break;
                }
        }
}
