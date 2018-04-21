/*!
 * \file parse.c
 * \brief Contains subroutines for parsing the input
 */
#include "amash.h"

int offset    = 0;
int offset_sc = 0;

/**
 *  mallocs a block of memory INPUT_LENGTH characters long
 *  @return Pointer to the block of memory
 */
char* new_string()
{
    char* x = (char*)malloc(sizeof(char) * INPUT_LENGTH);

    for (int i = 0; i < ARG_LENGTH; i++)
    {
        x[i] = '\0';
    }
    return x;
}


int count_blocks(char* input)
{
    char* i   = input;
    int count = 0;
    int c     = 0;

    //printf("Length = %d\n",strlen(input));
    for (c = 0; c < strlen(input); c++)
    {
        //printf("{%c}\n",i[0]);
        if (i[0] == '|')
        {
            count++;
            //printf("Match\n");
        }
        i++;
    }
    return count + 1;
}


int count_lines(char* input)
{
    char* i   = input;
    int count = 0;
    int c     = 0;

    //printf("Length = %d\n",strlen(input));
    for (c = 0; c < strlen(input); c++)
    {
        //printf("{%c}\n",i[0]);
        if (i[0] == ';')
        {
            count++;
            //printf("Match\n");
        }
        i++;
    }
    return count + 1;
}


char* extract(char* input)
{
    char* i = input + offset;
    //printf("offset = %d\nNEW : %s\n",offset,i);
    int  j = 0;
    char block[100];
    int  x = 0;

    for (x = 0; x < 100; x++)
    {
        block[x] = '\0';
    }
    while (1)
    {
        if ((i[0] != '|') & (i[0] != '\0') & (i[0] != '\n'))
        {
            //printf("{%c}\n",i[0]);
            block[j] = i[0];
            j++;
            i++;
        }
        else
        {
            offset = offset + j + 1;
            //printf("j = %d\n",j);
            //printf("off = %d\n__________________________\n",offset);
            return strdup(block);
        }
        //sleep(1);
    }
}


char* extract_line(char* input)
{
    char* i = input + offset_sc;
    //printf("offset = %d\nNEW : %s\n",offset,i);
    int  j = 0;
    char block[100];
    int  x = 0;

    for (x = 0; x < 100; x++)
    {
        block[x] = '\0';
    }
    while (1)
    {
        if ((i[0] != ';') & (i[0] != '\0') & (i[0] != '\n'))
        {
            //printf("{%c}\n",i[0]);
            block[j] = i[0];
            j++;
            i++;
        }
        else
        {
            offset_sc = offset_sc + j + 1;
            //printf("j = %d\n",j);
            //printf("off = %d\n__________________________\n",offset);
            return strdup(block);
        }
        //sleep(1);
    }
}


Executable* segment(char* input)
{
    char temp[100];
    int  match = 0;
    char * exe;
    char * argv[10];
    int  argc = 0;
    char * stdin;
    char * stdout;
    //Check stdin
    char* i = input;
    int j   = 0;
    int c;
    int x = 0;

    for (x = 0; x < 100; x++)
    {
        temp[x] = '\0';
    }
    for (c = 0; c < strlen(input); c++)
    {
        //printf("{%c}\n",i[0]);
        if ((i[0] == '>') | (i[0] == '\n'))
        {
            //printf("UnMatch1\n");
            match = 0;
        }
        if ((j > 0) & (i[0] == ' '))
        {
            //printf("UnMatch2\n");
            match = 0;
        }
        if (match == 1)
        {
            if (i[0] != ' ')
            {
                //printf("Copy\n");
                temp[j] = i[0];
                j++;
            }
        }
        if (i[0] == '<')
        {
            match = 1;
            //printf("Match\n");
        }
        i++;
    }

    stdin = strdup(temp);

    //log_debug("StdIn = %s\n", temp);
    x = 0;
    for (x = 0; x < 100; x++)
    {
        temp[x] = '\0';
    }
    //Check stdout
    i     = input;
    j     = 0;
    match = 0;
    for (c = 0; c < strlen(input); c++)
    {
        //printf("{%c}\n",i[0]);
        if ((i[0] == '<') | (i[0] == '\n'))
        {
            //printf("UnMatch1\n");
            match = 0;
        }
        if ((j > 0) & (i[0] == ' '))
        {
            //printf("UnMatch2\n");
            match = 0;
        }
        if (match == 1)
        {
            if (i[0] != ' ')
            {
                //printf("Copy\n");
                temp[j] = i[0];
                j++;
            }
        }
        if (i[0] == '>')
        {
            match = 1;
            //printf("Match\n");
        }
        i++;
    }
    stdout = strdup(temp);
    //log_debug("StdOut = %s\n", temp);
    for (x = 0; x < 100; x++)
    {
        temp[x] = '\0';
    }
    i     = input;
    j     = 0;
    match = 1;
    int top = 0;
    for (c = 0; c < strlen(input); c++)
    {
        if ((i[0] == '<') | (i[0] == '>'))
        {
            match     = 0;
            top       = 1;
            temp[j++] = ' ';
        }
        if ((i[0] != '<') & (i[0] != '>') & (i[0] != ' ') & (top == 1) & (match == 0))
        {
            top = 0;
        }
        if (match == 1)
        {
            //printf("Copy\n");
            temp[j] = i[0];
            j++;
        }
        if (((match == 0) & (top == 0) & (i[0] == ' ')) | ((match == 0) & (top == 0) & (i[0] == '<')) | ((match == 0) & (top == 0) & (i[0] == '>')))
        {
            match = 1;
        }
        i++;
    }
    //printf("*Execute = %s\n", temp);
    int  flag = 1;
    char arg[15];
    match = 0;
    int count = 0;
    for (c = 0; c < strlen(temp); c++)
    {
        //printf("{%c}\n",temp[c]);
        if ((temp[c] == ' ') | (temp[c] == '\n'))
        {
            match = 0;
        }
        else
        {
            match = 1;
            flag  = 0;
        }
        if ((match == 1) & (temp[c] != ' '))
        {
            //printf("Match\n");
            arg[count++] = temp[c];
        }
        if ((match == 0) & (flag == 0))
        {
            //printf("argc = %d\n",argc+1);
            arg[count++] = '\0';
            argv[argc]   = strdup(arg);
            argc++;
            flag = 1;
            for (x = 0; x < 15; x++)
            {
                arg[x] = '\0';
            }
            count = 0;
        }
    }
    if (strlen(arg) > 0)
    {
        //printf("argc = %d\n",argc+1);
        argv[argc] = strdup(arg);
        argc++;
    }
    //log_debug("argc = %d\n", argc);
    //log_debug("argv = ");
    for (c = 0; c < argc; c++)
    {
        //log_debug("%s ", argv[c]);
    }
    //log_debug("\n");
    exe = strdup(argv[0]);
    //log_debug("Exec = %s\n", exe);
    Executable* e = new_executable();
    strcpy(e->exec_path, exe);
    e->stdin = stdin;
    //printf("Copying %s to exectuable as stdin", stdin);
    e->stdout           = stdout;
    e->stderr_to_stdout = false;
    for (c = 0; c < argc; c++)
    {
        e->argv[c] = argv[c];
    }
    e->argc = argc;
    return e;
}


ParsedInput* parse(char* input)
{
    //log_debug("Piped_Count = %d\n", count_blocks(input));

    int        c = count_blocks(input);
    int        i;
    ParsedInput* p = new_parsedinput();

    for (i = 0; i < c; i++)
    {
        //log_debug("_____________________________\n\n");
        char* s = extract(input);
        //log_debug("[parse]Extracted input %s\n", s);
        Executable* e = new_executable();
        e = segment(s);                                                  //INDIVIDUAL EXECUTABLES : Each '|' seperated section
        memcpy(&p->executables[i], e, sizeof(Executable));
        //TODO Combine these executables<e> to ParsedInput
    }
    p->executables_count = c;
    offset = 0;
    return p;
}
