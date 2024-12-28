#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <stddef.h>
#include <stdio.h>

typedef struct node {
    void* value_;
    struct node* nextNode_;
} node;

typedef struct linked_list {
    struct node* firstNode_;
    struct node* lastNode_;
    int size_;                  //poc prvkov
    size_t elementSize_;        //velkost prvku
} linked_list;

typedef struct linkedList_iterator {
    linked_list* list_;
    node* current_;
} linkedList_iterator;

void linkedList_init(linked_list* this,size_t elementSize);
void linkedList_clear(linked_list* this);
void linkedList_destroy(linked_list* this);
int linkedList_size(linked_list* this);

void linkedList_addLast(linked_list* this, void* value);
void linkedList_addAtIndex(linked_list* this, void* value, int index);

void linkedList_deleteLast(linked_list* this);
void linkedList_deleteAtIndex(linked_list* this, int index);

_Bool linkedList_get(linked_list* this, int index, void* value);
void linkedList_set(linked_list* this, int index, void* value);

void linkedList_assign(linked_list* this, linked_list* other);

_Bool linkedList_saveToFile(linked_list* this, FILE* out);

_Bool linkedList_loadFromFile(linked_list* this, FILE* in);

void linkedList_exportToTxt(linked_list* this, FILE* out,const char* (*item_to_string)(const void* item));

void linkedListIterator_init(linkedList_iterator* this, linked_list *list, int index);
void linkedListIterator_destroy(linkedList_iterator* this);
void linkedListIterator_moveNext(linkedList_iterator* this);
void linkedListIterator_moveGet(const linkedList_iterator* this, void* value);
void linkedListIterator_begin(linked_list *list, linkedList_iterator *begin);

#endif //LINKED_LIST_H
