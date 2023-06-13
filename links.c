/**
 * IAED-23 Project 2
 * File: links.c
 * Author: Bibiana Andre ist194158
 * 
 * Description: file containing the implementation of link 
 * related functions.
*/

#include "main.h"


/* ----------------------------- Link functions ----------------------------- */

/**
 * Obtain the arguments read in the 'l' command line.
 * If the command line is successfully read and validated, returns a link 
 * with new data to be created. Else, returns NULL.
*/
Link* obtainLinkArgs(System *sys) {

    char line[BUFLEN], orig[BUFLEN], dest[BUFLEN], cost[BUFLEN], dura[BUFLEN];
    
    Link* new_link = (Link*)tryMalloc(sizeof(Link));

    /* Read line name. */
    if (getArg(sys, line)) {
        /* Read origin and destination stops name. */
        if (getArg(sys, orig) && getArg(sys, dest)) {
            /* Read cost and duration values. */
            if (getArg(sys, cost) && !getArg(sys, dura)) {

                /* Command line read successfully. */
                new_link = checkLinkArgsOK(sys, new_link, line, orig, dest);

                if (new_link != NULL) {

                    /* Convert strings to floating point variables. */
                    new_link->value.cost = atof(cost);
                    new_link->value.duration = atof(dura);
                    return new_link;
                } 
            }                  
        } 
    }

    free(new_link);
    return NULL;
}

/**
 * Check if the names of line, origin and destination read in the command line
 * are valid, returning the newly populated link if so. Else, returns NULL.
*/
Link* checkLinkArgsOK(System *sys, Link* new_link, char* line, char* orig, 
                                                                char* dest) {

    Line* line_ptr = getLine(sys, line);
    Stop *orig_ptr = getStop(sys, orig), *dest_ptr = getStop(sys, dest);

    if (line_ptr == NULL) {

        printf(NO_SUCH_LINE, line);
        return NULL;

    } else if (orig_ptr == NULL) {

        printf(NO_SUCH_STOP, orig);
        return NULL;

    } else if (dest_ptr == NULL) {

        printf(NO_SUCH_STOP, dest);
        return NULL;
    }

    /* Store data in the new link */
    return new_link = populateLink(new_link, line_ptr, orig_ptr, dest_ptr);
}

/**
 * Populates the given link with the given data. Returns the newly populated 
 * link.
*/
Link* populateLink(Link* new_link, Line* line, Stop* orig, Stop* dest) {

    new_link->line = (Line*)line;
    new_link->orig = (Stop*)orig;
    new_link->dest = (Stop*)dest;
    return new_link;
}

/**
 * Processes the data stored in the link. If the received line has no 
 * itinerary, it's created its first link. If the received origin stop is the
 * last stop of the line's itinerary, the link is appended to the line's route. 
 * If the received destination stop is the first stop of the line's itinerary, 
 * the link is pushed to the line's route. If none of the above, a warning 
 * message is presented and the task to create a new link is suspended.
*/
int processLinkData(Link* new_link) {
    
    Stop* origin = (Stop*)new_link->orig;
    Stop* destination = (Stop*)new_link->dest;
    Link *first, *last;

    if (((Line*)new_link->line)->num_stops == 0) 
        return FIRST_LINK; /* Create first link */

    first = (Link*)((Line*)new_link->line)->links_list->first->data;
    last = (Link*)((Line*)new_link->line)->links_list->last->data;

    if ((Stop*)last->dest == origin) 

        return APPEND; /* Append link to the line route */

    else if ((Stop*)first->orig == destination && origin != (Stop*)last->dest) 

        return PUSH; /* Push link to the line route */

    /* Link data is invalid to create a new link in the line. Suspend task. */
    return ERR;
}

/**
 * Creates the first link in a line after checking its values of cost 
 * and duration. If they happen to be negative, the link is not added.
*/
void addFirstLink(Link* new_link) {

    Line* line = (Line*)new_link->line;
    Stop* orig = (Stop*)new_link->orig;
    Stop* dest = (Stop*)new_link->dest;

    if (!assertNegativeValue(new_link->value)) {
        free(new_link);
        return;
    }

    line->total_value = new_link->value;
    line->num_stops = 2;

    append(line->links_list, (Link*)new_link);
    
    /* Associate line to origin stop. */
    addLineToStop(line, orig); 

    /* Associate line to destination stop. */
    if (orig != dest)
        addLineToStop(line, dest); 
}

/**
 * Adds a new link to the beginning of a line's route after checking its values
 * of cost and duration. If they happen to be negative, the link is not added.
*/
void linkPush(Link* new_link) {

    Line* line = (Line*)new_link->line;
    Stop* orig = (Stop*)new_link->orig;

    if (!assertNegativeValue(new_link->value)) {
        free(new_link);
        return;
    }

    line->total_value.cost += new_link->value.cost;
    line->total_value.duration += new_link->value.duration;
    line->num_stops += 1;

    /* Insert link in the beginning of the route. */
    push(line->links_list, (Link*)new_link);
    
    /* Associate line to origin stop */
    addLineToStop(line, orig); 
}

/**
 * Adds a new link to the end of a line's route after checking its values of 
 * cost and duration. If they happen to be negative, the link is not added.
*/
void linkAppend(Link* new_link) {

    Line* line = (Line*)new_link->line;
    Stop* dest = (Stop*)new_link->dest;

    if (!assertNegativeValue(new_link->value)) {
        free(new_link);
        return;
    }

    line->total_value.cost += new_link->value.cost;
    line->total_value.duration += new_link->value.duration;
    line->num_stops += 1;

    /* Insert link in the end of the route. */
    append(line->links_list, (Link*)new_link);

    /* Associate line to destination stop. */
    if (dest != (Stop*)new_link->orig)
        addLineToStop(line, dest); 
}

/**
 * Associates the given line with the given stop. 
*/
void addLineToStop(Line* line, Stop* stop) {

    if (searchList(stop->lines, (Line*)line) == NULL) 
        append(stop->lines, line);
}

/**
 * Assert if the input-read link values of cost and duration are positive,
 * returning YES if so. Else, returns NO.
*/
int assertNegativeValue(Values values) {

    if (values.cost < 0 || values.duration < 0) {

        printf(NEGATIVE_VALUE);
        return NO;
    }

    return YES;
}


/* -------------------- Link removal auxiliary functions -------------------- */

/**
 * Removes and deletes the given link from the given line, also removing the 
 * given node from its links list. Total cost and duration of the line
 * decrease. Auxiliary function to remove both lines and stops.
*/
void removeLink(Line* line, Link* to_delete, Node* node) {

    if (node != NULL) {

        /* Cost and duration decrease */
        line->total_value.cost -= to_delete->value.cost;
        line->total_value.duration -= to_delete->value.duration;

        listRemoveNode(line->links_list, node);
        free(to_delete);
        line->num_stops -= 1;
        
        if (line->num_stops <= 1) 
            line->num_stops = 0;
    }
}

/**
 * Deletes the association between the given line and the given stop.
 * Auxiliary function to remove both lines and stops.
*/
void removeLineFromStop(Line* line, Stop* stop) {

    listRemoveData(stop->lines, (Line*)line);
}