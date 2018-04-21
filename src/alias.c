/*!
 * \file alias.c
 * \brief Contains subroutines for setting, getting, and processing aliases.
 */
#include "amash.h"

char* get_alias(char* key)
{
    for (int i = 0; i < MAX_PAIRS; i++)
    {
        if (!aliases->is_set[i])
        {
            continue;
        }

        if (strcmp(key, aliases->keys[i]) == 0)
        {
            return aliases->values[i];
        }
    }
    return NULL;
}


void set_alias(char* key, char* value)
{
    int pos = aliases->insert_position;

    aliases->keys[pos]       = key;
    aliases->values[pos]     = value;
    aliases->is_set[pos]     = true;
    aliases->insert_position = (aliases->insert_position + 1) % MAX_PAIRS;
}


// TODO make this not crap
char* resolve_input(char* input)
{
    log_debug("Resolving input string: '%s'", input);
    char* output = input;

    for (int i = 0; i < MAX_PAIRS; i++)
    {
        if (!aliases->is_set[i])
        {
            continue;
        }

        if (starts_with(aliases->keys[i], input))
        {
            output = calloc(sizeof(char), strlen(input) + strlen(aliases->values[i]));
            strcpy(output, aliases->values[i]);

            strcat(output, input + 1 + strlen(aliases->keys[i]));
        }
    }
    log_debug("Resolved to: '%s'", output);
    return output;
}
