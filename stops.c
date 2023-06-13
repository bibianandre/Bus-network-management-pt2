/**
 * IAED-23 Project 2
 * File: stops.c
 * Author: Bibiana Andre ist194158
 * 
 * Description: file containing the implementation of stop 
 * related functions.
*/

#include "main.h"


/* ----------------------------- Stop functions ----------------------------- */

/**
 * List all the existing stops in the standard output, in the order they were 
 * created, presenting the name, coordinates and respective number of lines.
*/
void listStops(System *sys) {

    List* list = sys->stops_list;
    Node* ptr;
	Stop* stop;

    if (list->count != 0) {
        for (ptr = list->first; ptr != NULL; ptr = ptr->next) {

            stop = (Stop*)ptr->data;

            printf("%s: %16.12f %16.12f %d\n", stop->name, stop->latitude, 
                                        stop->longitude, stop->lines->count);
        }
    }
}

/**
 * Shows the coordinates of a given stop in the standard output. 
 * If the stop does not exist, a warning message is presented.
*/
void showStop(System *sys, char* name) {

    Stop* stop = getStop(sys, name);

    if (stop == NULL) {

        printf(NO_SUCH_STOP, name);
        return;

    } else {

        printf("%16.12f %16.12f\n", stop->latitude, stop->longitude);
    }
}

/**
 * Check if the stop with the given name exists in the stops hashtable.
 * If the name is found, returns a pointer to that stop. Else, returns NULL.
*/
Stop* getStop(System *sys, char* name) {

    HashElem* element = hashtableGet(sys->stops_table, name, getStopName);
    Node* stop_node;

    if (element == NULL) 
        return NULL;
    
    stop_node = (Node*)element->data;
    return (Stop*)stop_node->data;
}

/**
 * Retrieves the name of a stop.
*/
char* getStopName(void* stop) {

    Node* stop_node = stop;
    Stop* stop_get = (Stop*)stop_node->data;
    return stop_get->name;
}

/**
 * Adds (if possible) a new, non existing stop to the system.
*/
void addStop(System *sys, char* name, double latitude, double longitude) {

    Stop* new_stop = (Stop*)tryMalloc(sizeof(Stop));
    Node* new_node;

    if (getStop(sys, name) != NULL) {

        printf(STOP_ALREADY_EXISTS, name);
        free(new_stop);
        return;
    }

    new_stop = populateStop(new_stop, name, latitude, longitude);

    /* Saves stop in the stop list and saves the node in the hashtable. */
    new_node = listInsertEnd(sys->stops_list, new_stop);
    sys->stops_table = hashtableInsert(sys->stops_table, new_node, 
                                    getStopName(new_node), getStopName);
}

/**
 * Populates the given stop with the given data. Returns the newly 
 * populated stop.
*/
Stop* populateStop(Stop* new_stop, char* name, double lat, double lon) {

    new_stop->name = (char*)tryMalloc(sizeof(char) * (strlen(name) + 1));
    strcpy(new_stop->name, name);

    new_stop->lines = createList();
    new_stop->latitude = lat;
    new_stop->longitude = lon;

    return new_stop;
}

/**
 * Presents the lines that intersect a given stop in alphabetic order.
 * Auxiliary function to the 'i' command.
*/
void showStopLines(Stop* stop) {

    Node* ptr;
    Line* line;
    List* list = stop->lines;

    sortList(list, compareLines);
    line = (Line*)list->first->data;
    printf("%s", line->name);

    for (ptr = list->first->next; ptr != NULL; ptr = ptr->next) {

        line = (Line*)ptr->data;
        printf(" %s", line->name);
    }

    putchar('\n');
}

/**
 * Compares the names of two lines. Returns 1 if the second line name comes 
 * first in alphabetic order, 0 if the names are identical or -1 if the first 
 * line name comes first in alphabetic order. Auxiliary function to the 
 * 'showStopLines' function.
 */
int compareLines(void* first, void* second) {
    
	Line* first_line = first;
	Line* second_line = second;

	return strcmp(first_line->name, second_line->name);
}


/* ------------------------- Stop removal functions ------------------------- */

/**
 * Stop removal main function. Removes a stop from the system. Removes and/or
 * rearranges the links of the lines associated with the stop.
*/
void removeStop(System *sys, char* name) {

    HashElem* element = hashtableGet(sys->stops_table, name, getStopName);
    Node* stop_node;
    Stop* to_remove;

    if (element == NULL) {
        printf(NO_SUCH_STOP, name);
        return;
    }

    stop_node = (Node*)element->data;
    to_remove = (Stop*)stop_node->data;
    
    rearrangeAll(sys, to_remove);

    hashtableRemove(sys->stops_table, name, getStopName);
    listRemoveNode(sys->stops_list, stop_node);
    deleteStop(to_remove);
}

/**
 * Rearranges the link connections of all the system's lines associated with 
 * the given stop (if the stop has lines associated).
*/
void rearrangeAll(System *sys, Stop* stop) {

    Line* to_rearrange;
    Node* ptr;

    if (stop->lines->count != 0) {

        for (ptr = sys->lines_list->first; ptr != NULL; ptr = ptr->next) {

            to_rearrange = (Line*)ptr->data;

            if (to_rearrange->num_stops != 0)
                rearrangeLine(to_rearrange, stop);
        }
    }
}

/**
 * Rearranges the links connections of the given line to skip the given stop
 * in their itinerary. Removes the association between the line and the stop.
*/
void rearrangeLine(Line* line, Stop* stop) {

    Link *first = (Link*)line->links_list->first->data, *last;
    
    /* Delete links from the beginning of the route. */
    if ((Stop*)first->orig == stop) 
        deleteStopFromBeginning(line, stop);

    if (line->links_list->count != 0) {

        last = (Link*)line->links_list->last->data;

        /* Delete links from the end of the route. */
        if ((Stop*)last->dest == stop) 
            deleteStopFromEnd(line, stop);
    }

    /* Rearranges the remaining links. */
    if (line->links_list->count >= 2) {

        deleteStopFromMiddle(line, stop); 
        updateLineValues(line);
    }
        
    removeLineFromStop(line, stop);
}

/**
 * Deletes links from the beginning of the line's itinerary associated with 
 * the given stop as origin stop. Decreases the total cost and duration of the
 * line accordingly.
*/
void deleteStopFromBeginning(Line* line, Stop* stop) {

    Link *current = (Link*)line->links_list->first->data;
    Node *ptr1 = line->links_list->first, *ptr2;

    while ((Stop*)current->orig == stop) {

        ptr2 = ptr1->next;
        removeLink(line, current, ptr1);

        if (ptr2 == NULL) 
            break;
        
        ptr1 = ptr2;
        current = (Link*)ptr1->data;
    }  
}

/**
 * Deletes links from the end of the line's itinerary associated with the 
 * given stop as destination stop. Decreases the total cost and duration 
 * of the line accordingly.
*/
void deleteStopFromEnd(Line* line, Stop* stop) {

    Link *current = (Link*)line->links_list->last->data;
    Node *ptr1 = line->links_list->last, *ptr2;

    while ((Stop*)current->dest == stop) {

        ptr2 = ptr1->prev;
        removeLink(line, current, ptr1);

        if (ptr2 == NULL)
            break;
        
        ptr1 = ptr2;
        current = (Link*)ptr1->data;
    }  
}

/**
 * Rearranges the internal links of the route of the given line to skip the 
 * given stop. The links associated with the stop as origin stop are removed 
 * and their values of cost and duration are collapsed into the previous link 
 * (with the stop as destination stop). After each iteration, the destination 
 * stop of the current link becomes the removed link's destination stop.
*/
void deleteStopFromMiddle(Line* line, Stop* stop) {

    Link *new, *old, *next;
    Node *ptr1 = line->links_list->first, *ptr2;

    while (ptr1 != NULL) {

        old = (Link*)ptr1->data; 

        if ((Stop*)old->dest == stop) { 
            
            ptr2 = ptr1->next; 
            next = (Link*)ptr2->data;

            new = updateLink(new, old, next);

            removeLink(line, next, ptr2);

            if ((Stop*)new->dest != stop) 
                ptr1 = ptr1->next;

        } else {

            ptr1 = ptr1->next;
        }
    }
}

/**
 * Update the current link with the data of the next link (to be deleted).
*/
Link* updateLink(Link* new, Link* old, Link* next) {

    new = old; /* Current link. */
    new->dest = (Stop*)next->dest; /* Update current link destination. */
    new->value.cost += next->value.cost; /* Add cost of next link. */
    new->value.duration += next->value.duration; /* Add duration of next link.*/

    return new;
}

/**
 * Update the line's values of cost and duration after rearranging 
 * its itinerary.
*/
void updateLineValues(Line* line) {

    Node* ptr;
    Link* link;

    double total_cost = 0.00, total_duration = 0.00;

    for (ptr = line->links_list->first; ptr != NULL; ptr = ptr->next) {

        link = (Link*)ptr->data;
        total_cost += link->value.cost;
        total_duration += link->value.duration;
    }

    line->total_value.cost = total_cost;
    line->total_value.duration = total_duration;
}

/**
 * Frees all the allocated memory in the given stop.
*/
void deleteStop(void* stop) {

    Stop* to_delete = (Stop*)stop;
    listDestroy(to_delete->lines);
    free(to_delete->name);
    free(to_delete);
}
