The Amadeus Shell
=======

A basic shell implementation, written in C.

## File structure
All of the related files are located in the `src` folder.
The API structures and functions are defined in `amash.h`.
The definitions are split across various files, loosely grouped by function:
1. `amash.c` contains the main control functions
2. `exec.c` contains the functions to run programs
3. `parse.c` contains the functions to parse input strings  

## Building
This project utilizes the `Meson` build system, and requires meson and ninja-build to be installed.
These should be available on any major Linux repository.
If you are on Windows, you're on your own.  

To build it:

1. Create a build folder and cd to it.
```
mkdir build; cd build;
```
2. Initialize the meson system.
```
meson ..
```
3. Run ninja.
```
ninja build
```


## Working
This program's functionality is divided into 3 main parts:

### Main
The general algorithm of the event loop is:
1. read input from readline
2. parse INPUT
3. exec input
4. goto 1

See `run_event_loop`.

### Parse
The parser is a fairly basic DFA implementation. It scans the input left to right, one character at a time.
State is tracked with an enum(`parse_mode`).
As input is detected, the DFA transitions between states and performs certain actions.
At the end of the parse(state END), the final `ParsedInput` object is retured.
This is implemented in `parse`.

### Exec
To run a program, these things happen:
1. The program forks.
2. The new forked program sets its current file handles as specified.
3. The new program runs the target program with an exec system call.
This is implemented in `exec_program` for a single program, and `exec` for a chain of executables.

## About the name
The word `amadeus` derives from a latin word that roughly translates to `love God`.   
It is named after the memory storage and artificial intelligence system if the same name, developed by `Makise Kurisu` at Victor Chondria University.
The system was designed to convert human memory data to digital data and to emulate a human brain.

## Why are all the arrays statically allocated?

1. It's faster that way, since it avoids memory allocation overhead.
2. I can't possibly imagine a path longer than 100 characters, in any but the most exceptional circumstances.
3. I'm lazy.
