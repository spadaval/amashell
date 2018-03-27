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
This code works through

## About the name
The word `amadeus` derives from a latin word that roughly translates to `love God`.   
It is named after the memory storage and artificial intelligence system if the same name, developed by `Makise Kurisu` at Victor Chondria University.
The system was designed to convert human memory data to digital data and to emulate a human brain.

## Why are all the arrays statically allocated?

1. It's faster that way, since it avoids memory allocation overhead.
2. I can't possibly imagine a path longer than 100 characters, in any but the most exceptional circumstances.
3. I'm lazy.
