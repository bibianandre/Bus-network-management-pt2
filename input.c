/**
 * IAED-23 Project 2
 * File: input.c
 * Author: Bibiana Andre ist194158
 * 
 * Description: file containing the implementation of input related functions.
*/

#include "main.h"


/* ----------------------------- Input functions ---------------------------- */

/**
 * Indicates the end of input. If an '\n' is read, return NO. Else, return YES.
*/
int hasArgs(System* sys) {

    int c = getchar();

    sys->command_lenght--;

    if (c == '\n' || c == EOF) {

        sys->command_lenght = BUFLEN; /* Reset command length. */
        return NO;
    }
        
    return YES;   
}

/**
 * Reads a string containing white spaces from the standard input. As characters
 * are read, the command length decreases. If an '\n' char is read, resets the
 * command length and returns 0. Else, returns 1 without reseting the command 
 * length.
*/
int getArgWithSpaces(System* sys, char* str) {

    int c, i = 0, len = sys->command_lenght;

    sys->command_lenght--; 

    while ((c = getchar()) != '"' && i < len) {

        str[i++] = c; 
        sys->command_lenght--; 
    }

    sys->command_lenght--; 
    str[i] = '\0';

    if (!hasArgs(sys)) { /* End of input */

        return 0;

    } else if (sys->command_lenght == 0) { /* Max length reached. */

        sys->command_lenght = BUFLEN; /* Reset command length. */
        return 0;
    }

    return 1;
}

/**
 * Reads a string without white spaces from the standard input. As characters
 * are read, the command length decreases. If an '\n' char is read, resets the
 * command length and returns 0. Else, returns 1 without reseting the command 
 * length. The variable 'ch' is the first char read.
*/
int getArgWithNoSpaces(System* sys, char* str, int ch) {

    int i = 0;
    long int len = sys->command_lenght;

    while (!isspace(ch) && i < len) {

        str[i++] = ch;
        sys->command_lenght--;
        ch = getchar();
    }

    str[i] = '\0';
          
    if (ch == '\n' || sys->command_lenght == 0){ /* End of input */

        sys->command_lenght = BUFLEN; /* Reset command length. */
        return 0;
    }  
    
    return 1;
}

/**
 * Reads a string from the standard input. As characters are read, the command
 * length decreases. If an '\n' char is read, resets the command length and 
 * returns 0 as result. Else, returns 1 as result, without reseting the command 
 * length.
*/
int getArg(System* sys, char* string) {

    int c = getchar(), result;
    
    /* If white spaces are read */
    while(isspace(c) && c != '\n') {
        
        sys->command_lenght--;    
        c = getchar(); 
    }

    /* Argument can contain white spaces */
    if (c == '"') 
        return result = getArgWithSpaces(sys, string);

    /* Argument without spaces (including numeric values) */ 
    return result = getArgWithNoSpaces(sys, string, c);
}

/**
 * Reads the remaining command line buffer until the limit of 
 * characters is reached, or and '\n' char is read.
*/
void untilEndOfLine(System* sys) {

    while(hasArgs(sys) &&  sys->command_lenght > 0) { }
}