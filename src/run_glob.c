#include "glob.h"

//Main
int main(void)
{

        char* results[MAX_ENTRY];
        int glob_count = 0;
        glob_count = glob("/home/ako/us*",results);
        glob_count = glob("/home/ako/u*p",results);
        return 0;
}
