/**
 * IAED-23 Project 2
 * File: structures.c
 * Author: Bibiana Andre ist194158
 * 
 * Description: file containing the implementation of double linked lists,
 * merge sort and hashtables.
*/

#include "main.h"


/* ------------------------- Double linked lists ------------------------- */

/**
 * Creates a new empty list and returns its respective pointer.
 */
List* createList() {
    
	List* new_list = (List*)tryMalloc(sizeof(List));
	new_list->first = NULL;
	new_list->last = NULL;
	new_list->sorted = UNSORTED;
	new_list->count = 0;

	return new_list;
}

/**
 * Allocates and creates a new node to be added to a list.
*/
Node* addNode(Node* prev, Node* next, void* data) {

	Node* new_node = (Node*)tryMalloc(sizeof(Node));
	new_node->data = data;
	new_node->prev = prev;
	new_node->next = next;

	if (prev != NULL) 
		prev->next = new_node;

	if (next != NULL) 
		next->prev = new_node;

	return new_node; 
}

/**
 * Inserts new (previously allocated) data in the end of the given double 
 * linked list and rearranges the node connections. Returns a pointer to 
 * the new node to be saved in the respective hashtable.
 */
Node* listInsertEnd(List* list, void* data) {

    Node* new_node = addNode(list->last, NULL, data);

	list->last = new_node;

	if (list->first == NULL)
		list->first = new_node;

	if (list->sorted != UNSORTED)
		list->sorted = UNSORTED;

	list->count++;

	return new_node;
}

/**
 * Inserts new (already allocated) data in the end of the given 
 * double linked list and rearranges the node connections. 
 */
void append(List* list, void* data) {

	Node* new_node = addNode(list->last, NULL, data);

	list->last = new_node;

	if (list->first == NULL)
		list->first = new_node;

	if (list->sorted != UNSORTED)
		list->sorted = UNSORTED;

	list->count++;
}

/**
 * Inserts new (previously allocated) data in the beggining of the given 
 * double linked list and rearranges the node connections. Returns a 
 * pointer to the new node to be saved in the respective hashtable.
 */
Node* listInsertBeginning(List* list, void* data) {

    Node* new_node = addNode(NULL, list->first, data);

	list->first = new_node;

	if (list->last == NULL)
		list->last = new_node;

	if (list->sorted != UNSORTED) 
		list->sorted = UNSORTED;

	list->count++;

	return new_node;
}

/**
 * Inserts new (already allocated) data in the beginning of the 
 * given double linked list and rearranges the node connections. 
 */
void push(List* list, void* data) {

	Node* new_node = addNode(NULL, list->first, data);

	list->first = new_node;

	if (list->last == NULL)
		list->last = new_node;

	if (list->sorted != UNSORTED) 
		list->sorted = UNSORTED;

	list->count++;
}

/**
 * Searches for the node with the given data in the given list. If the 
 * node is found, returns a pointer to that node. Else, returns NULL.
*/
Node* searchList(List* list, void* data) {

	Node *ptr;

	for (ptr = list->first; ptr != NULL; ptr = ptr->next) {

		if (ptr->data == data) 
			return ptr;
	}

	return NULL;
}

/**
 * Removes the node with the given data from the given double linked 
 * list and rearranges the node connections. Does not delete the data.
 */
void listRemoveData(List* list, void* data) {

	Node* node = searchList(list, data);

	listRemoveNode(list, node);
}

/**
 * Removes the given node from the given double linked list and rearranges 
 * the node connections. Does not delete the data. Useful when different nodes 
 * may be storing the same data (links, for example, may have duplicate stops 
 * and values).
 */
void listRemoveNode(List* list, Node* node) {

	if (node != NULL) {
		
		if (node->prev != NULL) 
			node->prev->next = node->next;
		else 
			list->first = node->next;

		if (node->next != NULL) 
			node->next->prev = node->prev;
		else 
			list->last = node->prev;

		free(node);

		list->count--;
	}
}

/**
 * Frees the allocated memory of a given list. The nodes 
 * must be previously freed.
 */
void listDestroy(List* list) {

	free(list);
}


/* ----------------------------- Merge Sort ------------------------------ */

/**
 * Sorts a given double linked list using mergesort implementation and 
 * the given compare function, then marks the list as sorted. After the 
 * sorting, sets the new tail node.
 */
void sortList(List* list, int(*cmp)(void*, void*)) {

	Node* ptr;

	if (list->first == NULL || list->sorted == SORTED) 
		return;
	else if (list->sorted != SORTED) 
		list->sorted = SORTED;

	list->first = mergesortList(list->first, cmp);

	for (ptr = list->last; ptr->next != NULL; ptr = ptr->next) { }

	list->last = ptr;
}

/**
 * Merges two sorted double linked lists by rearranging the node 
 * connections given the compare function.
 */
Node* mergeSorted(Node* first, Node* second, int(*cmp)(void*, void*)) {

    if (first == NULL) 
        return second; 

    if (second == NULL) 
        return first; 

    /* Get the smaller value. */
    if (cmp(first->data, second->data) < 0) {

        first->next = mergeSorted(first->next, second, cmp);
		first->next->prev = first;
		first->prev = NULL;
        return first;

    } else {

        second->next = mergeSorted(first, second->next, cmp);
		second->next->prev = second;
		second->prev = NULL;
        return second;
    }
}

/**
 * Mergesort a double linked list.
 */
Node* mergesortList(Node* head, int(*cmp)(void*, void*)) {

	Node *second;

    if (head == NULL || head->next == NULL) 
        return head;

    second = splitList(head);

    /* Recursive mergesort for left and right halves. */
    head = mergesortList(head, cmp);
    second = mergesortList(second, cmp);

	/* Merges the two sorted halves. */
    return mergeSorted(head, second, cmp); 
}

/**
 * Splits a double linked list in two double linked lists in the given 
 * position, each list being half the size of the original one.
 */
Node* splitList(Node* head) {

    Node *fast = head, *slow = head, *tmp;

	/* Reaches the middle position of the list */
    while (fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }

    tmp = slow->next;

    slow->next = NULL;

    return tmp;
}


/* ------------------------------ Hashtable ------------------------------ */

/**
 * Creates and initializes a new hashtable with the given size, 
 * returning a pointer to it.
 */
Hashtable* createHashtable(int size) {

	int i;

	Hashtable* new_table = (Hashtable*)tryMalloc(sizeof(Hashtable));

	new_table->table = (HashElem**)tryMalloc(size * sizeof(HashElem*));
	new_table->size = size;
	new_table->elem_num = 0;

	for(i = 0; i < size; i++) 
		new_table->table[i] = NULL;

	return new_table;

}

/**
 * Allocates the necessary memory to create a hashtable element to store the 
 * given data in it. The element's state is either of taken or deleted, 
 * being created with the taken state.
 */
HashElem* createHashtableElement(void* data) {

	HashElem* elem = (HashElem*)tryMalloc(sizeof(HashElem));

	elem->data = data;
	elem->state = HT_TAKEN;
	return elem;
}

/**
 * Calculates the position to store the data with the given key. If the        
 * hashtable reaches its threshold, then it'll be expandeded. Since it 
 * can expand, it always returns a pointer to the table.
 */
Hashtable* hashtableInsert(Hashtable* hash, void* data, char* key, 
                                            char*(*get_key)(void*)) {
	
	int i = 1;
	unsigned int* hashes = calcHashtableHashes(key, hash->size);
	unsigned int h = hashes[0] % hash->size, phi = hashes[2];

	/* Find the spot to insert the new data */
	while (hash->table[h] != NULL) {

		if (hash->table[h]->state == HT_DELETED) {
			free(hash->table[h]);
			break;
		}

		h = (hashes[0] + i * phi) % hash->size;
		i++;
	}

	hash->table[h] = createHashtableElement(data);
	free(hashes);

	/* Expand the hashtable if necessary */
	if (++hash->elem_num >= hash->size * HT_MAX_LOAD) 
		hash = expandHashtable(hash, get_key);

	return hash;
}

/**
 * Expands the hashtable, creating a new hashtable with close to double 
 * the size and reinserting all of the info in the new table. Returns a 
 * pointer to the new expanded table.
 */
Hashtable* expandHashtable(Hashtable* hash, char*(*get_key)(void*)) {

	int i;
	Hashtable* new_hash = createHashtable(getPrime(hash->size * 2));

	for (i = 0; i < hash->size; i++) {

    if (hash->table[i] != NULL && hash->table[i]->state != HT_DELETED) {

			hashtableInsert(new_hash, hash->table[i]->data, 
                            get_key(hash->table[i]->data), get_key);
		}
	}

	destroyHashtable(hash);
	return new_hash;
}

/**
 * Receives a key to spot the position of the data in the hashtable.
 * If the data is found, returns a pointer to the hash element storing 
 * the data. Else, returns NULL.
 */
HashElem* hashtableGet(Hashtable* hash, char* key, char*(*get_key)(void*)) {

	int i = 1;
	unsigned int* hashes = calcHashtableHashes(key, hash->size);
	unsigned int h = hashes[0] % hash->size, hash_1 = hashes[0];
	unsigned int phi = hashes[2];

	free(hashes);

	/* Spot the position of the data in the hashtable */
	while (hash->table[h] != NULL) {

		if (hash->table[h]->state != HT_DELETED && 
            strcmp(get_key(hash->table[h]->data), key) == 0) {

			return hash->table[h];
		}

		h = (hash_1 + i * phi) % hash->size;
		i++;
	}

	return NULL;
}

/**
 * Receives an element to delete from the hashtable and mark it as deleted.
 */
void hashtableRemove(Hashtable* hash, char* key, char*(*get_key)(void*)) {

	HashElem* elem = hashtableGet(hash, key, get_key);

	if (elem == NULL) {
		return;
	}

	elem->state = HT_DELETED;
	--hash->elem_num;
}

/**
 * Clears all of the memory occupied by the hashtable and its elements.
 */
void destroyHashtable(Hashtable* hash) {

	int i;

	for (i = 0; i < hash->size; i++) {

		if (hash->table[i] != NULL) 
			free(hash->table[i]);
	}

	free(hash->table);
	free(hash);
}

/**
 * The hashtable element is dead when it's empty or has been deleted, 
 * returning YES if so. If the element is "alive", returns NO.
 */
int isElemDead(HashElem* elem) {

	if (elem == NULL || elem->state == HT_DELETED) 
		return YES;

	return NO;
}

/**
 * Calculates the hash given a key string (based on DJB algorithm) 
 * and returns the hash.
 */
unsigned int calculateHash(char* key) {

	unsigned int hash = 5381;
	int c;

	while((c = *(key++)) != 0)
		hash = ((hash << 5) + hash) + c;

	return hash;
}

/**
 * Calculate the hash step for a given key string and returns the 
 * hash step. (Based on Jenkins hash function).
 */
unsigned int calcHashStep(char* key) {

	unsigned int hash = 0;
	int i = 0;

	while (key[i] != '\0') {
		hash += key[i++];
		hash += hash << 10;
		hash ^= hash >> 6;
	}

	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;
	return hash;
}

/**
 * Calculates the two hashes for double hashing implementation, given 
 * the key string and the hashtable size. Calculates the phi value, 
 * used to scale the hashtable's main hash. Returns the double hash.
 */
unsigned int* calcHashtableHashes(char* key, int size) {

	unsigned int* hashes = (unsigned int*)tryMalloc(sizeof(unsigned int)*3);

	hashes[0] = calculateHash(key); /* First hash */
	hashes[1] = calcHashStep(key); /* Second hash */
	hashes[2] = hashes[1] % size; /* Phi */

	/* If phi is zero, reset to 1 */
	if (hashes[2] == 0) {
		hashes[2] = 1;
	}

	return hashes;
}

/**
 * Checks if the given number is prime. Returns YES if so and NO if else.
 */
int isPrime(int x) {

	int i;

	for (i = 2; i * i <= x; i++) {

		if (x % i == 0)
			return NO;
	}

	return YES;
}

/**
 * Returns the next prime number after the given number.
 */
int getPrime(int num) {

	int prime = num;

	while (!isPrime(prime))
		prime++;

	return prime;
}