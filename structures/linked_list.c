#include "linked_list.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void linkedList_init(linked_list *this,size_t elementSize) {
    this->size_ = 0;
    this->firstNode_ = NULL;
    this->lastNode_ = NULL;
    this->elementSize_ = elementSize;
}

void linkedList_clear(linked_list *this) {
    if (this->firstNode_ != NULL) {
        node *delNode = this->firstNode_;
        node *nextNode = this->firstNode_;
        while (delNode != NULL) {
            nextNode = nextNode->nextNode_;
            free(delNode->value_);
            free(delNode);
            delNode = nextNode;
        }
        this->firstNode_ = NULL;
        this->lastNode_ = NULL;
        this->size_ = 0;
        this->elementSize_ = 0;
    }
}

void linkedList_destroy(linked_list *this) {
    linkedList_clear(this);
}

int linkedList_size(linked_list *this) {
    return this->size_;
}

void linkedList_addLast(linked_list *this, void *value) {
    node *newNode = (node *)malloc(sizeof(node));
    newNode->value_ = malloc(this->elementSize_);
    memcpy(newNode->value_, value, this->elementSize_);
    newNode->nextNode_ = NULL;

    if (this->firstNode_ == NULL) {
        this->firstNode_ = newNode;
        this->lastNode_ = newNode;
    } else {
        this->lastNode_->nextNode_ = newNode;
        this->lastNode_ = newNode;
    }
    this->size_++;
}

void linkedList_addAtIndex(linked_list *this, void *value, int index) {
    if (index > this->size_ || index < 0) {
        printf("Index out of bounds\n");
    } else if (index == this->size_) {
        linkedList_addLast(this, value);
    } else {
        node* prevNode = this->firstNode_;
        for (int i = 0; i < index - 1; i++) {
            prevNode = prevNode->nextNode_;
        }
        node *newNode = (node *)malloc(sizeof(node));
        newNode->value_ = malloc(this->elementSize_);
        memcpy(newNode->value_, value, this->elementSize_);
        newNode->nextNode_ = prevNode->nextNode_;
        prevNode->nextNode_ = newNode;
        this->size_++;
    }
}

void linkedList_deleteLast(linked_list *this) {
    if (this->firstNode_ == NULL) {
        printf("List is empty\n");
    } else if (this->lastNode_ == this->firstNode_) {
        free(this->firstNode_->value_);
        free(this->firstNode_);
        this->firstNode_ = NULL;
        this->lastNode_ = NULL;
        this->size_ = 0;
    } else {
        node *last = this->firstNode_;
        for(int i = 0; i < this->size_ - 2; i++) {
            last = last->nextNode_;
        }
        free(this->lastNode_->value_);
        free(this->lastNode_);
        this->lastNode_ = last;
        this->lastNode_->nextNode_ = NULL;
        this->size_--;
    }
}

void linkedList_deleteAtIndex(linked_list *this, int index) {
    if (index > this->size_ || index < 0) {
        printf("Index out of bounds\n");
    } else if (index == this->size_ - 1) {
        linkedList_deleteLast(this);
    } else {

        node* prevNode = this->firstNode_;
        for (int i = 0; i < index - 1; i++) {
            prevNode = prevNode->nextNode_;
        }

        node* p = prevNode->nextNode_;
        prevNode->nextNode_ = prevNode->nextNode_->nextNode_;
        free(p->value_);
        free(p);
        this->size_--;
    }
}

_Bool linkedList_get(linked_list* this, int index, void* value) {
    if (index > this->size_ || index < 0) {
        printf("Index out of bounds\n");
        return false;
    }
    node* n = this->firstNode_;
    for (int i = 0; i < index; i++) {
            n = n->nextNode_;
        }
    memcpy(value, n->value_,this->elementSize_);
    return true;
}

void linkedList_set(linked_list *this, int index, void *value) {
    if (index >= this->size_ || index < 0) {
        printf("Index out of bounds\n");
    } else {
        node* n = this->firstNode_;
        for (int i = 0; i < index; i++) {
            n = n->nextNode_;
        }
        memcpy(n->value_, value, this->elementSize_);
    }
}

void linkedList_assign(linked_list *this, linked_list *other) {
    if (this != other) {
        linkedList_clear(other);
        node *n = this->firstNode_;
        while (n != NULL) {
            void *copiedValue = malloc(this->elementSize_);
            memcpy(copiedValue, n->value_, this->elementSize_);
            linkedList_addLast(other, copiedValue);
            n = n->nextNode_;
        }
    }
}

bool linkedList_saveToFile(linked_list *this, FILE *out) {
    fwrite(&this->size_, sizeof(this->size_), 1, out);
    fwrite(&this->elementSize_, sizeof(this->elementSize_), 1, out);
    node* n = this->firstNode_;
    while (n != NULL) {
        fwrite(n->value_, this->elementSize_, 1, out);
        n = n->nextNode_;
    }
    return true;
}

bool linkedList_loadFromFile(linked_list *this, FILE *in) {
    linkedList_clear(this);
    fread(&this->size_, sizeof(this->size_), 1, in);
    fread(&this->elementSize_, sizeof(this->elementSize_), 1, in);

    for (int i = 0; i < this->size_; i++) {
        void* value = malloc(this->elementSize_);
        fread(value, this->elementSize_, 1, in);
        linkedList_addLast(this, value);
        this->size_--;
        free(value);
    }
    return true;
}

void linkedList_exportToTxt(linked_list *this, FILE *out, const char *(*item_to_string)(const void *item)) {
    fprintf(out, "Size: %d \nElement size: %d \n", this->size_, this->elementSize_);
    node* n = this->firstNode_;
    while (n->nextNode_ != NULL) {
        const char* string = item_to_string(n->value_);
        fprintf(out, "%s, ", string);
        free((void*)string);
        n = n->nextNode_;
    }
    const char* string = item_to_string(n->value_);
    fprintf(out, "%s", string);
    free((void*)string);
}

void linkedListIterator_init(linkedList_iterator *this, linked_list *list, int index) {
    this->list_ = list;
    this->current_ = list->firstNode_;
    for (int i = 0; i < index && this->current_ != NULL; i++) {
        this->current_ =  this->current_->nextNode_;
    }
}

void linkedListIterator_destroy(linkedList_iterator *this) {
    this->list_ = NULL;
    this->current_ = NULL;
}

void linkedListIterator_moveNext(linkedList_iterator *this) {
    if (this->current_ != NULL) {
        this->current_ = this->current_->nextNode_;
    }
}

void linkedListIterator_moveGet(const linkedList_iterator *this, void *value) {
    if (this->current_ != NULL) {
        memcpy(value, this->current_->value_, sizeof(int)); //sizeof(this->current_->value)
    }
}

void linkedListIterator_begin(linked_list *list, linkedList_iterator *begin) {
    begin->list_ = list;
    begin->current_ = list->firstNode_;
}