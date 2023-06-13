#ifndef MAIN_H
#define MAIN_H

/**
 * IAED-23 Project 2
 * File: main.h
 * Author: Bibiana Andre ist194158
 * 
 * Description: main header file for the 2nd Project. Includes 
 * constants, prototypes, messages and structs.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "structures.h"


/* ------------------------------- Constants -------------------------------- */

        
/* Array sizes */
#define BUFLEN 65535        /* Starting length of command line. */

/* Words */
#define SORT "inverso"      /* Sort option input. */

/* Others */
#define ERR -1              /* If an error occurs or a task is interrupted. */
#define NO 0                /* If a statement if false. */
#define YES 1               /* If a statement is true. */
#define FIRST_LINK 2        /* To create the first link. */
#define APPEND 3            /* To append a new link to a list. */
#define PUSH 4              /* To push a new link into a list. */

                                
/* -------------------------------- Warnings -------------------------------- */


#define NO_MEMORY "No memory.\n" 
#define WRONG_SORT "incorrect sort option.\n" 
#define STOP_ALREADY_EXISTS "%s: stop already exists.\n" 
#define NO_SUCH_STOP "%s: no such stop.\n"
#define NO_SUCH_LINE "%s: no such line.\n" 
#define CANT_LINK "link cannot be associated with bus line.\n" 
#define NEGATIVE_VALUE "negative cost or duration.\n"


/* ------------------------------- Structures ------------------------------- */


/* Structure of values. */
typedef struct {
    double cost;
    double duration;
} Values;

/* Structure of stop. */  
typedef struct {              
    char* name;
    double longitude;
    double latitude;
    List* lines;
} Stop;

/* Structure of line. */
typedef struct {              
    char* name;
    int num_stops;  
    List* links_list;              
    Values total_value;
} Line;

/* Structure of link. */
typedef struct { 
    void *line, *orig, *dest;                 
    Values value;             
} Link;

/* Structure of global system. */
typedef struct {  
    long int command_lenght;    /* Current length of command line. */         
    List* lines_list;           /* To store lines by order of creation. */
    List* stops_list;           /* To store stops by order of creation. */
    Hashtable* stops_table;     /* To store all the stops by their name. */
    Hashtable* lines_table;     /* To store all the lines by their name. */ 
} System;


/* ------------------------------- Prototypes ------------------------------- */


/* system.c */

void* tryMalloc(unsigned int alloc);

System* systemInit();

void exitProgram(System *sys);

void clearSystem(System* sys);

void removeAllLinesSystem(System *sys);

void removeAllStopsSystem(System *sys);


/* input.c */

int hasArgs(System* sys);

int getArgWithSpaces(System* sys, char* str);

int getArgWithNoSpaces(System* sys, char* str, int ch);

int getArg(System* sys, char* string);

void untilEndOfLine(System* sys);


/* project2.c */

int handleCommand(System *sys);

void handleLineCommand(System *sys);

void handleStopCommand(System *sys);

void handleLinkCommand(System *sys);

void handleIntersectionCommand(System *sys);

void handleRemoveLineCommand(System *sys);

void handleRemoveStopCommand(System *sys);

void handleClearSystemCommand(System *sys);


/* lines.c */

void listLines(System *sys);

void addShowLine(System *sys, char* name, int sort);

Line* getLine(System *sys, char* name);

char* getLineName(void* line);

void addLine(System *sys, char* name);

Line* populateLine(Line* new_line, char* name);

void printLineStops(Line *line, int sort);

int assertSortOption(char opt[]);

void removeLine(System *sys, char* name);

void removeAllLinksLine(Line* line);

void removeLineAssociation(Line* line, Link* to_remove);

void emptyLineRoute(Line* line);

void deleteLine(void* line);


/* stops.c */

void listStops(System *sys);

void showStop(System *sys, char* name);

Stop* getStop(System *sys, char* name);

char* getStopName(void* stop);

void addStop(System *sys, char* name, double latitude, double longitude);

Stop* populateStop(Stop* new_stop, char* name, double lat, double lon);

void showStopLines(Stop* stop);

int compareLines(void* first, void* second);

void removeStop(System *sys, char* name);

void rearrangeAll(System *sys, Stop* stop);

void rearrangeLine(Line* line, Stop* stop);

void deleteStopFromBeginning(Line* line, Stop* stop);

void deleteStopFromEnd(Line* line, Stop* stop);

void deleteStopFromMiddle(Line* line, Stop* stop);

Link* updateLink(Link* new, Link* old, Link* next);

void updateLineValues(Line* line);

void deleteStop(void* stop); 


/* links.c */

Link* obtainLinkArgs(System *sys);

Link* checkLinkArgsOK(System *sys, Link* link, char* line, char* orig, 
                                                            char* dest);

Link* populateLink(Link* new_link, Line* line, Stop* orig, Stop* dest);

int processLinkData(Link* new_link);

void addFirstLink(Link* new_link);

void linkPush(Link* new_link);

void linkAppend(Link* new_link);

int assertNegativeValue(Values values);

void addLineToStop(Line* line, Stop* stop);

void removeLink(Line* line, Link* to_delete, Node* node);

void removeLineFromStop(Line* line, Stop* stop);


#endif
