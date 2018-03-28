/*!
   \file structs.c
   \brief Contains subroutines for creating and manipulating objects of the various
          structs in the project.
 */
#include "amash.h"

Executable *new_executable()
{
    Executable *e = (Executable *)malloc(sizeof(Executable));

    for (int i = 0; i < PATH_LENGTH; i++)
    {
        e->exec_path[i] = '\0';
    }

    for (int i = 0; i < MAX_ARGUMENTS; i++)
    {
        e->argv[i] = NULL;
    }


    e->argc = 0;

    return e;
}

ParsedInput *new_parsedinput()
{
    ParsedInput *p = (ParsedInput *)malloc(sizeof(ParsedInput));

    p->executables_count = 0;

    return p;
}

void dump_executable(Executable *e)
{
    if (e == NULL)
    {
        log_error("Received null executable!!");
    }

    log_trace("Dumping executable:");
    char arguments_string[100];
    strcpy(arguments_string, "'");
    strcat(arguments_string, e->argv[0]);
    strcat(arguments_string, "'");

    for (int i = 1; i < e->argc; i++)
    {
        strcat(arguments_string, ",'");
        strcat(arguments_string, e->argv[i]);
        strcat(arguments_string, "'");
    }
    log_debug("Executable(path = '%s', argc=%d, argv=(%s), stdin={%s}, stdout={%s}, redirect_stderr={%d})", e->exec_path, e->argc, arguments_string
              , e->stdin, e->stdout, e->stderr_to_stdout);
}
