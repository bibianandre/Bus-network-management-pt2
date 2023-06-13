/**
 * IAED-23 Project 2
 * File: system.c
 * Author: Bibiana Andre ist194158
 * 
 * Description: file containing the implementation of the program's main 
 * system functions.
*/

#include "main.h"


/* ---------------------------- System functions ---------------------------- */

/**
 * Controlled malloc. Whenever memory is allocated using malloc, it's checked 
 * if there is spare memory. If there's no memory left, an error message is 
 * presented and the program is exited with the respective error code.
 */
void* tryMalloc(unsigned int alloc) {

	void* p = malloc(alloc);

	if (p != NULL) {

		return p;

	} else {

		printf(NO_MEMORY);
		exit(ERR);
	}

	return NULL;
}

/**
 * Initializes the program's global system.
 */
System* systemInit() {

    System *new_system = (System*)tryMalloc(sizeof(System));

    new_system->command_lenght = BUFLEN;

    new_system->lines_list = createList();
    new_system->stops_list = createList();
    new_system->stops_table = createHashtable(HT_START_SIZE);
    new_system->lines_table = createHashtable(HT_START_SIZE);

    return new_system;

}

/**
 * Frees all the allocated memory in the program before exiting.
 * Deletes the main system.
 */
void exitProgram(System *sys) {
    
    clearSystem(sys);

    listDestroy(sys->lines_list);
    listDestroy(sys->stops_list);
    destroyHashtable(sys->lines_table);
    destroyHashtable(sys->stops_table);

    free(sys);
}

/**
 * Clears the memory allocated in the system's main structures.
 * Does not delete the structures.
 */
void clearSystem(System* sys) {

    /* Empties the main lines list and hashtable. */
    if (sys->lines_list->count != 0) 
        removeAllLinesSystem(sys);
    
    /* Empties the main stops list and hashtable. */
    if (sys->stops_list->count != 0)
        removeAllStopsSystem(sys);
}

/**
 * Removes all lines from the system, emptying the system's list of lines
 * and respective hashtable.
*/
void removeAllLinesSystem(System *sys) {

    List* list = sys->lines_list;
    Node *ptr, *aux;
    Line* to_delete;

    for (ptr = list->first; ptr != NULL; ptr = aux) {

        aux = ptr->next;
        to_delete = (Line*)ptr->data;
        removeLine(sys, to_delete->name);
    }
}

/**
 * Removes all stops from the system, emptying the system's list of 
 * stops and respective hashtable.
*/
void removeAllStopsSystem(System *sys) {

    List* list = sys->stops_list;
    Node *ptr, *aux;
    Stop* to_delete;

    for (ptr = list->first; ptr != NULL; ptr = aux) {

        aux = ptr->next;
        to_delete = (Stop*)ptr->data;
        removeStop(sys, to_delete->name);
    }
}