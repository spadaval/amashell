/*!
 * \file alias.c
 * \brief Contains subroutines for setting, getting, and processing environ.
 */
#include "amash.h"

char* get_env(char* key)
{
    for (int i = 0; i < MAX_environ; i++)
    {
        if (!environ->is_set[i])
        {
            continue;
        }

        if (strcmp(key, environ->keys[i]) == 0)
        {
            return environ->values[i];
        }
    }
    return NULL;
}


void set_env(char* key, char* value)
{
    int pos = environ->insert_position;

    environ->keys[pos]       = key;
    environ->values[pos]     = value;
    environ->is_set[pos]     = true;
    environ->insert_position = (environ->insert_position + 1) % MAX_environ;
}
