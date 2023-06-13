#ifndef STRUCTURES_H
#define STRUCTURES_H

/**
 * IAED-23 Project 2
 * File: structures.h
 * Author: Bibiana Andre ist194158
 * 
 * Description: header file for structures.c file. Includes constants, 
 * prototypes and structs.
*/

/* ------------------------------- Constants -------------------------------- */


#define HT_MAX_LOAD 0.5	    /* Hashtable max load. */
#define HT_START_SIZE 20047 /* Starting size of hashtable. */
#define HT_DELETED -8	    /* If the hash element state is of deleted. */
#define HT_TAKEN -9	        /* If the hash element state is of taken. */
#define SORTED -10	        /* If a double linked list is sorted. */
#define UNSORTED -11        /* If a double linked list is unsorted. */


/* -------------------------------- Structs --------------------------------- */


/* Structure of hash element */
typedef struct hash_elem_t {
    void* data;
    int state;
} HashElem;

/* Structure of hashtable */
typedef struct hashtable_t {
    int size;
    int elem_num;
    struct hash_elem_t** table;
} Hashtable;

/* Structure of double linked list */
typedef struct list_t {
    struct node_t *first;
    struct node_t *last;
    int count;
    int sorted;
} List;

/* Structure of node */
typedef struct node_t {
    void* data;
    struct node_t *prev;
    struct node_t *next;
} Node;


/* ------------------------------- Prototypes ------------------------------- */


/* Double linked lists */

List* createList();

Node* addNode(Node* prev, Node* next, void* data);

Node* listInsertEnd(List* list, void* data);

void append(List* list, void* data);

Node* listInsertBeginning(List* list, void* data);

void push(List* list, void* data);

Node* searchList(List* list, void* data);

void listRemoveData(List* list, void* data);

void listRemoveNode(List* list, Node* node);

void listDestroy(List* list);


/* Merge sort */

void sortList(List* list, int(*cmp)(void*, void*));

Node* mergeSorted(Node* first, Node* second, int(*cmp)(void*, void*));

Node* mergesortList(Node* head, int(*cmp)(void*, void*));

Node* splitList(Node* head);


/* Hashtables */

Hashtable* createHashtable(int size);

Hashtable* hashtableInsert(Hashtable* hash, void* data, char* key, 
                                            char*(*get_key)(void*));

Hashtable* expandHashtable(Hashtable* hash, char*(*get_key)(void*));

HashElem* hashtableGet(Hashtable* hash, char* key, char*(*get_key)(void*));

void hashtableRemove(Hashtable* hash, char* key, char*(*get_key)(void*));

void destroyHashtable(Hashtable* hash);

int isElemDead(HashElem* elem);

unsigned int calculateHash(char* key);

unsigned int calcHashStep(char* key);

unsigned int* calcHashtableHashes(char* key, int size);

int isPrime(int x);

int getPrime(int num);


#endif