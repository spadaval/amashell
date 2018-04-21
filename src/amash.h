/*!
   \file amash.h
   \brief Contains definitions for all the structs and functions in the shell
 */
#ifndef AMASH_H
#define AMASH_H 1

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "log.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define PATH_LENGTH 100
#define MAX_ARGUMENTS 10
#define ARG_LENGTH 100
#define INPUT_LENGTH 1000
#define MAX_EXECUTABLES 10
#define MAX_ALIASES 30
#define MAX_LINES 100

extern int offset;
extern int offset_sc;

//////////////
//structs.c //
//////////////

//NB make sure argv is always null-terminated
/**
 *  \struct Executable
 *  \brief Represents one single executable to run, containing the path,
 *         arguments, and redirects.
 */
typedef struct Executable {
        char exec_path[PATH_LENGTH];/*!< string of the exectuable to run, may or may not be relative */
        char* stdin;        /*!< What should stdin be set to? (null =>not used) */
        char* stdout;       /*!< What should stdout be set to? (null =>not used)*/
        bool stderr_to_stdout; /*!< Should stderr be set to stdout? */
        char* argv[ARG_LENGTH]; /*!< Argument strings to pass */
        int argc;               /*!< Number of arguments being passed */
} Executable;

/**
 *  Create a new struct Executable object
 *  @return A blank struct Executable object
 */
Executable* new_executable();

/**
 *  Log the details of an executable
 *  @param e Executable object to display
 */
void dump_executable(Executable* e);

/**
 *  \struct ParsedInput
 *  \brief A chain if executables to run
 */
typedef struct ParsedInput {
        Executable executables[MAX_ARGUMENTS];/*!< Chain of executables to run */
        int executables_count;               /*!< Number of executables currently in chain*/
} ParsedInput;

/**
 *  Create a new struct ParsedInput object
 *  @return A blank struct ParsedInput object
 */
ParsedInput* new_parsedinput();

/**
 *  Stores history in chronoogical order
 *
 */
struct history {
        char* lines[MAX_LINES];
        int lines_count;
} history;

void push_history(char* c);

void init_history();

void get_previous_line(char* c);

////////////
//parse.c //
////////////

/**
 *  Parse the received input, and produce a ParsedInput object.
 *  Utilizes a DFA. Scans the input left-to-right, one character at a time.
 *  @param  input input string
 *  @return A `ParsedInput` object
 */
ParsedInput* parse(char* input);

///////////
//exec.c //
///////////

/**
 *  Run an input string
 *  @param  input The input string to run
 *  @return       Return code
 */
int run_input(char *input);

/**
 *  Executes a single program in a new process(after forking). Uses execp to search PATH, if necessary.
 *  @param e Details of the executable
 */
void exec_program(Executable* e);

/**
 *  Executes a chain of programs.
 *  @param e A ParsedInput object
 */
void exec(ParsedInput* e);

/**
 *  Set the file handles of the current process according to the parameter.
 *  @param e Executable object, containing a list of redirects to apply.
 */
void set_redirects(Executable* e);

/**
 *  If the given command is a builtin, run the approproate function and return true.
 *  @param  e The given executable
 *  @return   true if a builtin was run, false if otherwise
 */
bool handle_builtins(Executable* e);

/**
 *  Run the cd builtin
 *  @param e Executable details
 */
void do_cd(Executable* e);
/**
 *  Run the pwd builtin
 *  @param e Executable details
 */
void do_pwd(Executable* e);
/**
 *  Run the quit builtin
 *  @param e Executable details
 */
void do_quit(Executable* e);

/**
 *  Run the history builtin, which prints recent history,  and searches through
 *  it, when given a parameter
 *  @param e Exectuable details
 */
void do_history(Executable* e);

/**
 *  Run the alias builtin command, which prints all set aliases, and sets an
 *  alias, if given a parameter
 *  @param e Executable details. If an argument is given (argc>1), alias will
 *  try to set an alias. Otherwise it lists all set aliases.
 */
void do_alias(Executable* e);

////////////
//amash.c //
////////////

/**
 *  Starts the main event loop
 *  @return exit code
 */
int run_event_loop();

/**
 *  Print the welcome prompt to STDOUT
 */
void printPrompt();

/**
 *  Test if str starts with pre
 *  @param  pre The prefix
 *  @param  str The string to test
 *  @return     true if str starts with pre, false otherwise
 */
bool starts_with(const char *pre, const char *str);

int count_lines(char* input);

char* extract_line(char* input);

void parser(char* input);

char prompt[200];   /*!< A string containing the prompt to display */

////////////
//alias.c //
////////////


typedef struct alias {
        char* keys[MAX_ALIASES];
        char* values[MAX_ALIASES];
        bool is_set[MAX_ALIASES];
        int insert_position;
} Alias;

Alias* aliases;

/**
 *  Get a certain alias, given a key
 *  @param  key What alias to look for
 *  @return     The aliased string
 */
char* get_alias(char* key);

/**
 *  Create a certain alias
 *  @param key   THe key of the alias
 *  @param value The replacement string
 */
void set_alias(char* key, char* value);

/**
 *  Scan an input string, replacing aliases as you go
 *  @param  input String to scan
 *  @return       Replaced string
 */
char* resolve_input(char* input);


////////////
//utils.c //
////////////

/**
 *  Checks if the last non-whitespace character in `line` is a slash, and replaces it with a null character
 *  @param  line Ths string to check
 *  @return      True if line ends with slash, false otherwise.
 */
bool line_ends_with_slash(char* line);

#endif
