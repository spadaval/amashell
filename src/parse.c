/*!
   \file parse.c
   \brief Contains subroutines for parsing the input
*/
#include "amash.h"

int slice_count(char* input)
{
        int i = 0, count = 0;
        while(input[i]!='\0')
        {
                if(input[i]==' ')
                {
                        count++;
                }
                i++;
        }
        log_debug("\tNumber of arguments = %d",count);
        return count;
}

Executable* parse_single(char* input)
{
        int offset = 0;
        int length = strlen(input);
        char temp[10][10];
        int i;

        Executable* e = new_executable();

        sscanf(input, "%s", e->exec_path);
        offset += strlen(e->exec_path);
        sscanf(e->exec_path, "%s", temp[0]);
        e->argc++;

        int arg_count = slice_count(input);

        if(arg_count==0)
        {
                //e->argv=NULL;
                e->argc=0;
        }
        else
        {
                for(i=0;i<arg_count;i++)
                {
                        sscanf((input+offset), "%s", temp[e->argc]);
                        offset += strlen(temp[e->argc++])+1;
                }
                for(i=0;i<=arg_count;i++)
                {
                        log_debug("%s",temp[i]);
                        e->argv[i] = strdup(temp[i]);
                }
        }
/*
        while(offset < length)
        {
            sscanf((input+offset), "%s", e->argv[e->argc]);
            offset += strlen(e->argv[e->argc++]);
        }
 */
        return e;
}

Executable* new_executable()
{
        Executable* e = (Executable*)malloc(sizeof(Executable));

        for(int i = 0; i<100; i++)
                e->exec_path[i] = '\0';
/*
        for(int i = 0; i<10; i++)
                e->redirects[i] = NULL;

        e->redirects_count = 0;
 */
        for(int i = 0; i<10; i++)
                e->argv[i] = '\0';

        e->argc = 0;

        return e;
}

/*
   ParsedInput* new_parsedinput()
   {
        assert(false);
        ParsedInput* p = (ParsedInput*)malloc(sizeof(ParsedInput));

        for(int i = 0; i<10; i++)
                p->executables[i] = NULL;

        p->executables_count = 0;

        for(int i = 0; i<10; i++)
                p->redirects[i] = NULL;

        p->redirects_count = 0;

        return p;
   }
 */
