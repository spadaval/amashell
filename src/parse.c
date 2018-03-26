/*!
   \file parse.c
   \brief Contains subroutines for parsing the input
 */
#include "amash.h"

Executable *parse_single(char *input)
{
    int offset = 0;
    int length = strlen(input);

    Executable *e = newExecutable();

    sscanf(input, "%s", e->exec_path);
    sscanf(input, "%s", e->argv[0]);
    e->argc = 1;
    offset += strlen(e->exec_path);

    while(offset < strlen(input)){
        sscanf(input+offset, "%s", e->argv[e->argc]);
        offset += strlen(e->argv[e->argc++]) + 1;
    }

    return e;
}
