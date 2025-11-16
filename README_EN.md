# Project for the data structures course
This project involved coding a processor (CPU) simulator in C language, capable of interpreting a simplified assembly language. The program manages different memory segments, a set of registers, several addressing modes, as well as the execution of classic instructions such as MOV, ADD, CMP, JMP, etc.
The goal was to reproduce a simplified version of how a CPU works, while respecting its memory management and sequential processing constraints.

Our project consists of 14 code files, a Makefile, and 3 text files:

## Project files

### 1. Files answering the questions in Exercise 1– Implementation of a generic hash table

'hash.h' and ‘hash.h’:
These files contain the declarations of the HashEntry and Hashmap structures,
the signatures of the functions that manipulate a Hashmap (creation, insertion, deletion, destruction, display of a Hashmap) and their definitions in .c.


### 2. Files answering the questions in Exercise 2– Dynamic memory management

‘memo.h’ and ‘memo.c’:
These files contain the declarations of the Segment and MemoryHandler structures, the signatures of the functions that manipulate these structures, and their definitions (in .c).

### 3. Files answering the questions in Exercise 3—Designing a parser for a pseudo-assembly language


'parser.h' and ‘parser.c’:
These files contain the declarations of the Instruction and ParserResult structures, the signatures of the functions that manipulate these structures, and their definitions (in .c).

### 4. Files answering the questions in Exercise 4– Allocation of a data segment

'cpu.h' and ‘cpu.c’:

These files contain the CPU structure declaration and the signatures and definitions of the functions that manipulate CPUs.


### 5. Files answering the questions in Exercise 5– Regular expressions and addressing resolution

‘regexadr.h’ and 'regexadr.c'

These files contain the signatures and declarations of the functions that identify the different addressing modes in pseudo-assembly language in order to determine how an operand should be interpreted.


### 6. Files answering the questions in Exercise 6—Code segment allocation and execution

‘segcode.h’ and 'segcode.c'

These files contain the signatures and declarations of the functions that allow our simulated CPU to execute code.

### 7. ‘main.c’

This file contains all the test sets related to our project. Four test sets were performed, each testing a part of the code. 
They are introduced as follows: '//TEST OF FILE.C'

### 8. ‘Makefile’
This file is used to automate the compilation of the project. It contains the rules necessary to compile the source files and generate the executable.

### 9. ‘test_parse.txt’ and ‘test_parse1.txt’
This file (assembler file) is used in the main program to create a parser.

### 10. ‘readme.txt’
This file (the current file) contains a description of the project and the files that comprise it.

### 11. ‘exo7.c’
This file contains the answers to exercise 7, which are normally present in other files, but due to lack of time we were unable to make it work properly. 

## Instructions for compiling and running the project

1. Remove the /* */ annotations associated with the test suite you want to try.
2. Run the ‘make all’ command to compile all the files in the project.
3. Run the generated executable with ‘./main’.

Annotations are present on all files to facilitate understanding.
In the main file in particular, each step of the test sets is explained.

(The annotations for the last test set (for segcode.c) are not present.)

We have provided two test files in assembly language, each of which can be tried. We have left the test on the most complete text file.
