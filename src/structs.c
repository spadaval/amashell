/*!
   \file structs.c
   \brief Contains subroutines for creating and manipulating objects of the various
          structs in the project.
*/
#include "amash.h"

Executable *newExecutable()
{
    Executable *e = (Executable *)malloc(sizeof(Executable));

    for (int i = 0; i < 100; i++)
    {
        e->exec_path[i] = '\0';
    }

    for (int i = 0; i < 10; i++)
    {
        e->argv[i] = '\0';
    }

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
