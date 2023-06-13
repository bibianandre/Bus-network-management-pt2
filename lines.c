/**
 * IAED-23 Project 2
 * File: lines.c
 * Author: Bibiana Andre ist194158
 * 
 * Description: file containing the implementation of line 
 * related functions.
*/

#include "main.h"


/* ----------------------------- Line functions ----------------------------- */

/**
 * List all the system's lines in the standard output, by order of creation, 
 * presenting the name, first and last stop (if the line has stops), number 
 * of stops, total cost and total duration of the itinerary.
*/
void listLines(System *sys) {

    Node* ptr;
    Stop *orig, *dest;
    Line* line;

    if (sys->lines_list->count != 0) {
        for (ptr = sys->lines_list->first; ptr != NULL; ptr = ptr->next) {

            line = (Line*)ptr->data;

            if (line->num_stops < 2) {

                printf("%s %d %.2f %.2f\n", line->name, line->num_stops,
                                            line->total_value.cost, 
                                            line->total_value.duration);
            } else {

                orig = (Stop*)((Link*)line->links_list->first->data)->orig;
                dest = (Stop*)((Link*)line->links_list->last->data)->dest;

                printf("%s %s %s %d %.2f %.2f\n", line->name,  
                                                    orig->name,
                                                    dest->name,
                                                    line->num_stops,
                                                    line->total_value.cost, 
                                                    line->total_value.duration);
            }
        }
    } 
}

/**
 * Adds the line (if it doesn't exist), or presents in the standard output 
 * the itinerary of the line with the given name according to the sort option. 
*/
void addShowLine(System *sys, char* name, int sort) {

    Line* line = getLine(sys, name);

    if (line == NULL) {

        addLine(sys, name); 

    } else if (line->num_stops != 0) {

        printLineStops(line, sort); 
    }
}

/**
 * Checks if the line with the given name exists in the system. If the 
 * name is found, returns a pointer to the line. Else, returns NULL.
*/
Line* getLine(System *sys, char* name) {

    HashElem* element = hashtableGet(sys->lines_table, name, getLineName);
    Node* line_node;

    if (element == NULL) 
        return NULL;

    line_node = (Node*)element->data;
    return (Line*)line_node->data;
}

/**
 * Retrieves the name of a line.
*/
char* getLineName(void* line) {

    Node* line_node = line;
    Line* line_get = (Line*)line_node->data;
    return line_get->name;
}

/**
 * Adds (if possible) a new, non existing line to the system. 
*/
void addLine(System *sys, char* name) {

    Line* new_line = (Line*)tryMalloc(sizeof(Line));
    Node* new_node;

    new_line = populateLine(new_line, name);
        
    /* Saves line in the line list and the respective node in the hashtable. */
    new_node = listInsertEnd(sys->lines_list, new_line);
    sys->lines_table = hashtableInsert(sys->lines_table, new_node, 
                                        getLineName(new_node), getLineName);

}

/**
 * Populates the given line with the given data. Returns the 
 * newly populated line.
*/
Line* populateLine(Line* new_line, char* name) {

    new_line->name = (char*)tryMalloc((strlen(name) + 1) * sizeof(char));
    strcpy(new_line->name, name);

    new_line->links_list = createList();
    new_line->num_stops = 0;
    new_line->total_value.cost = 0.00;
    new_line->total_value.duration = 0.00;

    return new_line;
}

/**
 * Shows the itinerary of a given line in the standard output, being the 
 * variable 'sort' the sort indicator: if YES, the itinerary is presented 
 * backwards; if NO, is presented as default. 
*/
void printLineStops(Line *line, int sort) {

    Node* ptr;
    Stop* current;

    if (!sort) {

        for (ptr = line->links_list->first; ptr != NULL; ptr = ptr->next)
            printf("%s, ", ((Stop*)((Link*)ptr->data)->orig)->name);

        current = (Stop*)((Link*)line->links_list->last->data)->dest;
        printf("%s\n", current->name);

    } else {

        for (ptr = line->links_list->last; ptr != NULL; ptr = ptr->prev) 
            printf("%s, ",((Stop*)((Link*)ptr->data)->dest)->name);

        current = (Stop*)((Link*)line->links_list->first->data)->orig;
        printf("%s\n", current->name);
    }
}

/**
 * Asserts if the input-read sort option is a valid one.
 * Returns YES if so. Returns NO otherwise.
*/
int assertSortOption(char opt[]) {

    char sort[] = SORT;
    int i, len = strlen(opt);

    if (len < 3 || len > 7) {

        printf(WRONG_SORT);
        return NO;
    }

    for (i = 0; i < len; i++) {

        if(opt[i] != sort[i]) {

            printf(WRONG_SORT);
            return NO;
        }
    }

    return YES;
}


/* ------------------------- Line removal functions ------------------------- */

/**
 * Line removal main function. Removes a line from the system. Removes all 
 * the links created in the given line, removing also its association with 
 * the respective stops.
*/
void removeLine(System *sys, char* name) {

    HashElem* element = hashtableGet(sys->lines_table, name, getLineName);
    Node* line_node;
    Line* to_remove; 

    if (element == NULL) {
        printf(NO_SUCH_LINE, name);
        return;
    }

    line_node = (Node*)element->data;
    to_remove = (Line*)line_node->data;

    removeAllLinksLine(to_remove);

    hashtableRemove(sys->lines_table, name, getLineName);
    listRemoveNode(sys->lines_list, line_node);
    deleteLine(to_remove);
}

/**
 * Deletes all links created in the given line, emptying its links list. 
*/
void removeAllLinksLine(Line* line) {

    Node* ptr;
    Link* link;

    if (line->links_list->count != 0) {
        
        for (ptr = line->links_list->first; ptr != NULL; ptr = ptr->next) {

            link = (Link*)ptr->data;

            removeLineAssociation(line, link);
        }

        emptyLineRoute(line);
    }
}

/**
 * Removes the association between the stops of the given link 
 * and the given line.
*/
void removeLineAssociation(Line* line, Link* to_remove) {

    Stop *orig, *dest;

    if (to_remove != NULL) {

        orig = (Stop*)to_remove->orig;
        dest = (Stop*)to_remove->dest;

        removeLineFromStop(line, orig);

        if (orig != dest)
            removeLineFromStop(line, dest);

        line->num_stops -= 1;

        if (line->num_stops <= 1) 
            line->num_stops = 0;
    }
}

/**
 * Clears all the links in the line's itinerary from the beginning and 
 * deletes each link by freeing its allocated memory.
*/
void emptyLineRoute(Line* line) {

    Link *current = (Link*)line->links_list->first->data;
    Node *ptr1 = line->links_list->first, *ptr2;

    while (ptr1 != NULL) {

        ptr2 = ptr1->next;

        removeLink(line, current, ptr1);

        ptr1 = ptr2;

        if (ptr1 != NULL)
            current = (Link*)ptr1->data;
    }  
}

/**
 * Frees all the allocated memory in the given line.
*/
void deleteLine(void* line) {

    Line* to_delete = line;
    listDestroy(to_delete->links_list);
    free(to_delete->name);
    free(to_delete);
}