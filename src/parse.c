/*!
   \file parse.c
   \brief Contains subroutines for parsing the input
 */
#include "amash.h"

/**
 *  mallocs a block of memory INPUT_LENGTH characters long
 *  @return Pointer to the block of memory
 */
char* new_string()
{
    char* x = (char*)malloc(sizeof(char)*INPUT_LENGTH);
    for(int i = 0; i < ARG_LENGTH; i++)
        x[i] = '\0';
    return x;
}

Executable *parse_single(char *input)
{
    int offset = 0;
    int length = strlen(input);

    Executable *e = new_executable();

    sscanf(input, "%s", e->exec_path);
    e->argv[0] = new_string();
    sscanf(input, "%s", e->argv[0]);
    e->argc = 1;
    offset += strlen(e->exec_path);

    while(offset < strlen(input)){
        e->argv[e->argc] = new_string();

        sscanf(input+offset, "%s", e->argv[e->argc]);
        offset += strlen(e->argv[e->argc++]) + 1;
    }
    e->argv[e->argc] = NULL;
    return e;
}
