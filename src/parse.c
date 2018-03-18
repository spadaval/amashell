/*!
   \file parse.c
   \brief Contains subroutines for parsing the input
*/
#include "amash.h"

//TODO Should we use Lex here, or just do it ourselves?
Executable* parse_single(char* input){
    log_debug("Parsing string: %s", input);
    int offset = 0;
    int length = strlen(input);

    Executable* e = new_executable();

    sscanf(input, "%s", e->exec_path);

    offset += strlen(e->exec_path);

    while(offset < length){
        sscanf((input+offset), "%s", e->argv[e->argc]);
        offset += strlen(e->argv[e->argc++]);
    }

    return e;
}

Executable* new_executable(){
    Executable* e = (Executable*)malloc(sizeof(Executable));

    for(int i = 0;i<100;i++)
        e->exec_path[i] = '\0';

    for(int i = 0;i<10;i++)
        e->redirects[i] = NULL;

    e->redirects_count = 0;

    for(int i = 0;i<10;i++)
        e->argv[i] = NULL;

    e->argc = 0;

    return e;
}
/*
ParsedInput* new_parsedinput(){
    assert(false);
    ParsedInput* p = (ParsedInput*)malloc(sizeof(ParsedInput));

    for(int i = 0;i<10;i++)
        p->executables[i] = NULL;

    p->executables_count = 0;

    for(int i = 0;i<10;i++)
        p->redirects[i] = NULL;

    p->redirects_count = 0;

    return p;
}
*/
