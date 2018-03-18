/*!
   \file amash.h
   \brief Contains all the major struct, union, and API function definitions.
   \author Sumeet Padavala
   \date "16"/"March"/"2018"
*/
#ifndef AMASH_H
#define AMASH_H 1

#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include "log.h"
/**
 *  \union Target
 *  \brief This union represents a target of a redirection.
 *         This can be either a file handle or a path name(relative or absolute)
 */
typedef union Target {
    char* path;             /*!< Path to the file */
    int file_handle;        /*!< file handle of the target */
} Target;

typedef struct Redirect{
    bool is_used;           /*!< Is this redirect supposed to applied? */
    bool target_is_path;    /*!< Is the target a path or file handle? */
    Target target;          /*!< target of the redirection (a union) */
} Redirect;

//TODO make sure argv is always null-terminated
typedef struct Executable{
    char exec_path[100];        /*!< string of the exectuable to run, may or may not be relative */
    Redirect* redirects[10];    /*!< list of redirects to apply for the first 10 file handles*/
    int redirects_count;
    char* argv[10];         /*!< Argument strings to pass */
    int argc;               /*!< Number of arguments being passed */
} Executable;

Executable* new_executable();

/**
 *  Parse the input string for a single executable, and return an Executable object
 *  @return An Executable object
 */
Executable* parse_single();

/**
 *  \struct ParsedInput
 *  \brief Represents one line of input after parsing(by the parse function).
 *         This operates similarly to the Executable, but alows you to treat a
 *         chain of piped executables as a single executable.
 */

// typedef struct ParsedInput{
//     Executable* executables[10];    /*!< List of executables to pipe to each other in sequence */
//     int executables_count; /*!< Number of executables */
//     char* env;              /*!< Environment variables to apply */
//     int env_count;          /*!< Number of environment variables */
// } ParsedInput;

// ParsedInput* new_parsedinput();



/**
 *  Parse the received input, and produce a ParsedInput object
 *  @param  input input string
 *  @return       An object representing a breakdown of the given command
 */
// ParsedInput* parse(char* input);

/**
 *  Executes parsed input, delegates most of the work to exec. Mostly concerned
 *  with pipes and stuff.
 *  @param i Parsed input
 */
// void exec_input(ParsedInput* i);

/**
 *  Executes a single program in a new process(after forking). Uses execp to search PATH, if necessary.
 *  @param e Details of the executable
 */
void exec_program(Executable* e);

/**
 *  Set the file handles of the current process according to the parameter.
 *  @param e Executable object, containing a list of redirects to apply.
 */
void set_redirects(Executable* e);

/**
 *  Starts the main event loop
 *  @return exit code
 */
int run_event_loop();


#endif
