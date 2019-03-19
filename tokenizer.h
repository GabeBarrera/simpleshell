/** 
 * TOKENIZER.H
 * 
 * Gabriel Barrera
 * Tara Vu
 * Professor Roch
 * CS 570
 * Due Date: Thursday 02/21/2019
 */

// CORE LIBRARIES
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

// CHANGE DIRECTORY 
#include <dirent.h>
#include <errno.h>

// WAIT
#include <sys/types.h>
#include <sys/wait.h>

/**
 * MAXARGS - global variable for args array
 * 
 * Set how many args can be stored with a command
 * before erroring out. MAXARGS allocates 1 arg
 * element for the command, 1 for a potential
 * delimiter, and the rest are allocated for 
 * arguments. So if you want to allow for 100 
 * arugments, the math is 100 + 2 = 102 as your
 * initilized integer.
 * 
 *     cmd       args     delimiter
 *   argv[0] + argv[...] + argv[n]
 *      1    +    n      +    1     = n+2
 */
#define MAXARGS 102
#define MAXWORD 100 // Char max per word

// GLOBAL VARIABLES
extern char * currentDir; // Stores current working directory
extern pid_t kid;         // PID int for forking processes
extern size_t size;       // Establishes size for getcwd()

/* LINKED LIST STRUCT
 * 
 * Note: Linked List will also be referred
 * to as LL from here on out.
 * 
 * Defines new struct 'token' that will
 * be known as 'token_node'. This struct
 * stores a tokenized string and a
 * pointer to the next node in LL.
 */
typedef struct token {
    char * word; //Tokenized String
    struct token * next; //Next Word Pointer
} token_node;