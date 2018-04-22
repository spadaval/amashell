//Preprocessor Directives
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>

//Memory Limits
#define PATH_SIZE 128
#define MAX_ENTRY 256
#define MAX_MATCHES 1
#define BUFFER_SIZE 32

//Configuration File
#define conf_file "./../src/wildcards.conf"

//Function Prototypes
void get_dir(char** path);
void display(char** path);
void set_path(char** path);
char* evaluate(char** dub_path);
int match(regex_t *pexp, char *sz);
int search(char* query, char** path, char** matches);
char* dvlp_reg(char* query,char* wildcards,char** actions);
void read_conf(char* wildcards,char** actions);
int glob(char* path, char** results);
