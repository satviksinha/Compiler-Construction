# Toy Compiler

This project is a compiler for a custom programming language. It includes a lexical analyzer, a parser, and generates a parse tree.

## Features

*   **Lexical Analysis:** Converts source code into a stream of tokens.
*   **Syntax Analysis:** Parses the token stream based on a defined grammar (see `grammar.txt`).
*   **Parse Tree Generation:** Creates and displays a parse tree from the source code.
*   **Comment Removal:** Utility to strip comments from the source code.
*   **Time Profiling:** Option to measure the time taken for compilation stages.

## Building and Running

1.  **Build the compiler:**
    ```bash
    make
    ```
2.  **Run the compiler:**
    The compiler takes three command-line arguments: the source code file, the output file for the parse tree, and the buffer size.
    ```bash
    ./driver <source_code_file> <parse_tree_output_file> <buffer_size>
    ```
    **Example using a test case:**
    ```bash
    ./driver testcase1.txt parsetree.txt 1024
    ```
    This will compile `testcase1.txt`, write the parse tree to `parsetree.txt`, and use a buffer size of 1024 bytes.

## Project Structure

```
.
├── .gitignore
├── driver.c           # Main driver program with interactive menu
├── grammar.txt        # Definition of the language grammar
├── lexer.c            # Lexical analyzer implementation
├── lexer.h            # Lexer interface
├── lexerDef.h         # Lexer definitions (tokens, etc.)
├── makefile           # Makefile for building the project
├── parser.c           # Parser implementation
├── parser.h           # Parser interface
├── parserDef.h        # Parser definitions
├── stackAdt.c         # Stack Abstract Data Type implementation
├── stackAdt.h         # Stack ADT interface
├── stackAdtDef.h      # Stack ADT definitions
├── testcase1.txt      # Example source code file
├── testcase2.txt      # Example source code file
├── testcase3.txt      # Example source code file
├── testcase4.txt      # Example source code file
├── testcase5.txt      # Example source code file
├── testcase6.txt      # Example source code file
├── tree.c             # Parse tree implementation
├── tree.h             # Parse tree interface
└── treeDef.h          # Parse tree definitions
```

## Contributors

*   Sarang Sridhar (2020A7PS0297P)
*   Kashish Mahajan (2020A7PS0995P)
*   Satvik Sinha (2020A7PS0993P)
*   Aarya Attrey (2020A7PS0036P)
*   Urvashi Sharma (2020A7PS0017P)

## Usage Options

After building and running the compiler with `./driver <source_file> <parsetree_output_file> <buffer_size>`, you will be presented with the following options:

1.  **Print comment-free code on terminal:** 
    *   Removes comments (enclosed in `** ... **`) from the source file and prints the result to the console.
2.  **Print token list on terminal:**
    *   Performs lexical analysis on the source file and prints the list of tokens with their line numbers, lexemes, and token names.
3.  **Print parse tree in text file:**
    *   Parses the source code and writes the parse tree to the specified output file (e.g., `parsetree.txt`). It will indicate if parsing was successful or if syntax errors were found.
4.  **Print total time taken by the stage 1 code:**
    *   Measures and prints the CPU time taken for the compilation process (lexical analysis, parsing, and parse tree generation).
0.  **Exit**

**Example Workflow:**

1.  Build the compiler:
    ```bash
    make
    ```
2.  Run the driver with a test case:
    ```bash
    ./driver testcase1.txt parsetree.txt 1024
    ```
3.  The program will then display the menu. If you enter `2`, it will print the token list for `testcase1.txt`. If you enter `3`, it will generate `parsetree.txt`.
