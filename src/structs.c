/*!
   \file structs.c
   \brief Contains subroutines for creating and manipulating objects of the various
          structs in the project.
*/
#include "amash.h"

Executable *new_executable()
{
    Executable *e = (Executable *)malloc(sizeof(Executable));

    for (int i = 0; i < 100; i++)
    {
        e->exec_path[i] = '\0';
    }

    for (int i = 0; i < MAX_ARGUMENTS; i++)
        e->argv[i] = NULL;


    e->argc = 0;

    return e;
}

Redirect *newRedirect()
{
    Redirect *r = (Redirect *)malloc(sizeof(Redirect));

    r->is_used = false;
    r->target_is_path = false;
    r->target = (Target *)malloc(sizeof(Target));
    return r;
}

Redirect *newRedirectFromFileHandle(int fd)
{
    Redirect *r = newRedirect();

    r->is_used = true;
    r->target_is_path = false;
    r->target->fd = fd;
    return r;
}

Redirect *newRedirectFromPath(char *path)
{
    Redirect *r = newRedirect();

    r->is_used = true;
    r->target_is_path = true;
    strcpy(r->target->path, path);
    return r;
}

void dump_executable(Executable *e)
{
    if(e == NULL)
        log_error("Received null executable!!");

    log_trace("Dumping executable:");
    char arguments_string[100];
    int offset = 0;
    strcpy(arguments_string, "'");
    strcat(arguments_string, e->argv[0]);
    strcat(arguments_string, "'");

    for (int i = 1; i < e->argc; i++)
    {
        strcat(arguments_string, ",'");
        strcat(arguments_string, e->argv[i]);
        strcat(arguments_string, "'");
    }
    log_debug("Executable(path = '%s', argc=%d, argv=(%s) )", e->exec_path, e->argc, arguments_string);
}
