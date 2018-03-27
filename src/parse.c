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
/**
 *  \enum parse_mode
 *  \brief  State numbers for the parser DFA
 */
enum parse_mode{
    START,
    EXEC_PATH,
    ARGUMENT,
    STDIN,
    STDOUT,
    WHITESPACE
};

/**
 *  \struct DFA
 *  \brief A simple container struct for the data of the DFA
 */
struct DFA{
    enum parse_mode curr_mode;
    char* curr_word_base, *curr_word;
    Executable* curr_executable;
    ParsedInput* output;
} dfa;

//TODO finish this
ParsedInput* parse(char *input)
{
    assert(input != NULL);

    // Initialize the DFA
    dfa.curr_mode = START;
    dfa.output = new_parsedinput();

    // begin parsing each character
    while(input++)
    {
        switch(dfa.curr_mode)
        {
            case START:
                dfa.curr_word_base = dfa.curr_word = new_string();
                dfa.curr_executable = new_executable();
                dfa.curr_mode = EXEC_PATH;
                break;

            case EXEC_PATH:
                switch(*input)
                {
                    case ' ':
                    case '<':
                    case '>':
                    case '|':
                    default: break;
                }
                break;

            case ARGUMENT:
                switch(*input)
                {
                    case ' ':
                    case '<':
                    case '>':
                    case '|':
                    default: break;
                }
                break;

            case STDIN:
                switch(*input)
                {
                    case ' ':
                    case '<':
                    case '>':
                    case '|':
                    default: break;
                }
                break;

                case STDOUT:
                    switch(*input)
                    {
                        case ' ':
                        case '<':
                        case '>':
                        case '|':
                        default: break;
                    }
                    break;

                case WHITESPACE:
                    switch(*input)
                    {
                        case ' ':
                        case '<':
                        case '>':
                        case '|':
                        default: break;
                    }
                    break;

        }
    }
}
