/**
 * IAED-23 Project 2
 * File: project2.c
 * Author: Bibiana Andre ist194158
 * 
 * Description: Main project file for the 2nd Project (Busline Management).
 * Includes command handling functions inplementation.
*/

#include "main.h"


/* ----------------------------- Handle command ----------------------------- */

/**
 * Handles command input.
 * Returns 1 if the program should continue after running the command.
 * Otherwise, returns 0.
 */
int handleCommand(System *sys) {

	char c = getchar();

	switch (c) {

		case 'c': handleLineCommand(sys);
			return 1;
		case 'p': handleStopCommand(sys);
			return 1;
		case 'l': handleLinkCommand(sys);
			return 1;
		case 'i': handleIntersectionCommand(sys);
			return 1;
        case 'r': handleRemoveLineCommand(sys);
            return 1;
        case 'e': handleRemoveStopCommand(sys);
            return 1;
        case 'a': handleClearSystemCommand(sys);
            return 1;
        case 'q': 
			return 0; /* Exit the program. */
		default:
			return 1; /* Ignore undefined commands. */
	}
}

/**
 * Handles the 'p' command.
*/
void handleStopCommand(System *sys) {

    char val1[BUFLEN], val2[BUFLEN], name[BUFLEN];
    double latitude, longitude;

    if (hasArgs(sys)) {

        if (!getArg(sys, name)) {

            showStop(sys, name);

        } else if (getArg(sys, val1) && !getArg(sys, val2)) {

            /* Convert strings to floating point variables */
            latitude = atof(val1);
            longitude = atof(val2);
            addStop(sys, name, latitude, longitude);
        }

    } else {

        listStops(sys);
    }   
}

/**
 * Handles the 'c' command.
*/
void handleLineCommand(System *sys) {

    char name[BUFLEN], option[BUFLEN];

    if (hasArgs(sys)) {

        if (!getArg(sys, name)) {

            addShowLine(sys, name, NO);

        } else if (!getArg(sys, option)) {

            if (getLine(sys, name) == NULL || strlen(option) == 0) {

                /* Line does not exist, so sort option is irrelevant... */
                /* ... Or read an empty word. */
                addShowLine(sys, name, NO);
                return;
            } 
                
            /* Check if the non-empty word is a valid sort option. */
            if (assertSortOption(option))
                addShowLine(sys, name, YES);        
        }

    } else {

        listLines(sys);
    }
}

/**
 * Handles the 'l' command.
*/
void handleLinkCommand(System *sys) {

    Link* new_link = obtainLinkArgs(sys);

    if (new_link != NULL) {
        
        switch(processLinkData(new_link)) {

            case FIRST_LINK: addFirstLink(new_link);
                break;
            case APPEND: linkAppend(new_link);
                break;
            case PUSH: linkPush(new_link);
                break;
            default: free(new_link);
                printf(CANT_LINK);
                break;
        }  
    }
}

/**
 * Handles the 'i' command.
*/
void handleIntersectionCommand(System *sys) {

    List* list = sys->stops_list;
    Node* ptr;
    Stop* stop;

    untilEndOfLine(sys);

    for (ptr = list->first; ptr != NULL; ptr = ptr->next) {

        stop = (Stop*)ptr->data;

        if (stop->lines->count > 1) {

            printf("%s %d: ", stop->name, stop->lines->count);
            showStopLines(stop);
        }
    }
}

/**
 * Handles the 'r' command.
*/
void handleRemoveLineCommand(System *sys) {

    char name[BUFLEN];

    if (!getArg(sys, name)) 
        removeLine(sys, name);
}

/**
 * Handles the 'e' command.
*/
void handleRemoveStopCommand(System *sys) {

    char name[BUFLEN];

    if (!getArg(sys, name)) 
        removeStop(sys, name);
}    

/**
 * Handles the 'a' command.
*/
void handleClearSystemCommand(System *sys) {

    untilEndOfLine(sys);
    clearSystem(sys);
}


/* ---------------------------------- Main ---------------------------------- */


int main() {

    System* sys = systemInit();
    
	/* Execute program until the user sends the 'q' command. */
	while (handleCommand(sys) > 0) { }

    exitProgram(sys);
    
	return 0;
}