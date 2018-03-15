# amashell
A basic shell implementation, written in C.

## File structure
All of the related files are located in the `src` folder.
The API structures and functions are defined in `amash.h`.
The definitions are split across various files, grouped by function.

## Building
This project utilizes the `Meson` build system, and requires meson and ninja to be installed.
These should be available on any major Linux repository. If you are on Windows, you're on your own.  
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
