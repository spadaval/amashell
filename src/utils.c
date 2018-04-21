#include "amash.h"

bool starts_with(const char* pre, const char* str)
{
    while (*pre != '\0')
    {
        if (*(pre++) != *(str++))
        {
            return false;
        }
    }
    return true;
}


/**
 *  Generate the prompt string, and copy it to `prompt`.
 */
void generatePrompt()
{
    char cwd[100];
    char login[100];
    char hostname[100];

    getcwd(cwd, sizeof(cwd));
    getlogin_r(login, sizeof(login));
    gethostname(hostname, sizeof(hostname));
    sprintf(prompt, "%s%s@%s%s:%s%s$%s ", KRED, hostname, KBLU, login, KYEL, cwd, KGRN);
}


bool line_ends_with_slash(char* line)
{
    int offset = strlen(line) - 1;

    while (offset >= 0)
    {
        switch (line[offset])
        {
        case ' ':
            line[offset] = '\0';
            break;

        case '/':
            line[offset] = '\0';
            return true;

        default:
            return false;
        }
    }
    return false;
}
