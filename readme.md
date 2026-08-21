# CPU Simulator in C

A simplified CPU simulator written in C that interprets a reduced assembly language. The program models core components of a processor: multiple memory segments, a register set, several addressing modes, and sequential execution of classic instructions such as `MOV`, `ADD`, `CMP`, and `JMP`.

The goal was to reproduce a simplified model of how a CPU operates, while respecting realistic constraints on memory management and sequential instruction processing.

## Overview

The project is built around a generic hash table, a dynamic memory handler, a parser for a pseudo-assembly language, and an execution engine. Together these components load a program from a text file, resolve its operands and addressing modes, and run it instruction by instruction on the simulated CPU.

## Project structure

The project consists of 14 source files, a `Makefile`, and 3 text files.

### Hash table (generic)
`hash.h` / `hash.c` — Declarations for the `HashEntry` and `Hashmap` structures, plus the functions that operate on a hashmap: creation, insertion, deletion, destruction, and display.

### Dynamic memory management
`memo.h` / `memo.c` — Declarations for the `Segment` and `MemoryHandler` structures and the functions that manipulate them.

### Pseudo-assembly parser
`parser.h` / `parser.c` — Declarations for the `Instruction` and `ParserResult` structures and the functions that build and manipulate them.

### CPU core
`cpu.h` / `cpu.c` — Declaration of the `CPU` structure and the functions that operate on it, including data segment allocation.

### Addressing modes (regular expressions)
`regexadr.h` / `regexadr.c` — Functions that identify the different addressing modes of the pseudo-assembly language, determining how each operand should be interpreted.

### Code segment and execution
`segcode.h` / `segcode.c` — Functions that allocate the code segment and allow the simulated CPU to execute a loaded program.

### Entry point and tests
`main.c` — Contains all test suites for the project. Four separate test suites each exercise a distinct part of the code. Each one is introduced by a comment of the form `// TEST DE FICHIER.C`.

### Build and data files
- `Makefile` — Automates compilation: contains the rules needed to compile the source files and produce the executable.
- `test_parse.txt` / `test_parse1.txt` — Sample assembly files used by `main.c` to build a parser. Two test files are provided; either can be used.
- `exo7.c` — Contains work for exercise 7, which was intended to be integrated into the other files. Due to time constraints, this part is not fully functional.

## Build and run

1. Uncomment the `/* */` block for the test suite you want to run.
2. Compile the project with `make all`.
3. Run the generated executable with `./main`.

The source files are commented throughout to make the code easier to follow. In `main.c` in particular, each step of every test suite is explained.

## Notes

- The comments for the final test suite (`segcode.c`) are not included.
- Two assembly test files are provided; the test left active in `main.c` uses the more complete one.

## Concepts covered

Generic hash tables, dynamic memory allocation and segment management, lexical parsing of an assembly-like language, addressing-mode resolution via regular expressions, and a sequential fetch–decode–execute cycle.

Nous avons fourni 2 fichiers test en langage assembleur, chacun peut-être essayé. Nous avons laissé le test sur le fichier texte le plus complet.


