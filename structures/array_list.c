#include "array_list.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_LIST_DEFAULT_CAPACITY 5

static void array_list_resize(array_list* this, int new_capacity) {
    this->array_ = realloc(this->array_, new_capacity * this->element_size_);
    this->capacity_ = new_capacity;
}

void array_list_init(array_list* this, size_t element_size) {
    this->element_size_ = element_size;
    this->capacity_ = ARRAY_LIST_DEFAULT_CAPACITY;
    this->size_ = 0;
    this->array_ = calloc(this->capacity_, this->element_size_);
}

void array_list_destroy(array_list* this) {
    free(this->array_);
    this->array_ = NULL;
    this->size_ = 0;
    this->capacity_ = 0;
    this->element_size_ = 0;
}

void array_list_clear(array_list* this) {
    this->size_ = 0;
}

int array_list_get_size(const array_list* this) {
    return this->size_;
}

void array_list_add(array_list* this, const void* value) {
    if (this->size_ >= this->capacity_) {
        array_list_resize(this, this->capacity_ > 0 ? this->capacity_ * 2 : ARRAY_LIST_DEFAULT_CAPACITY);
    }
    memcpy((char*)(this->array_) + this->size_ * this->element_size_, value , this->element_size_);
    this->size_++;
}

_Bool array_list_remove(array_list *this, int index) {
    if (index < 0 || index >= this->size_) {
        return false;
    }

    char* start = (char*)this->array_ + index * this->element_size_;
    char* next = start + this->element_size_;
    size_t bytes_to_move = (this->size_ - index - 1) * this->element_size_;

    memmove(start, next, bytes_to_move);
    this->size_--;
    return true;
}

_Bool array_list_try_get(const array_list* this, int index, void* value) {
    if (index <0 || index >= this->size_) {
        return false;
    }

    memcpy(value, (char*)(this->array_) + index * this->element_size_, this->element_size_);

    return true;
}

_Bool array_list_try_set(array_list* this, int index, const void* value) {
    if (index < 0 || index >= this->size_) {
        return false;
    }
    memcpy((char*)this->array_ + index * this->element_size_, value, this->element_size_);
    return true;
}

void array_list_assign(array_list* this, const array_list* other) {
    if (this != other) {
        this->capacity_ = other->capacity_;
        this->size_ = other->size_;
        this->element_size_ = other->element_size_;
        array_list_resize(this, other->capacity_);
        memcpy(this->array_, other->array_, this->size_ * this->element_size_);
    }
}

void array_list_process_all(const array_list* this, void (*process_item)(void* item)) {
    char* item = this->array_;
    const char* end = this->array_ + this->size_ * this->element_size_;
    while (item < end) {
        process_item(item);
        item += this->element_size_;
    }
}

void array_list_sort(array_list *this, int(*compare)(const void *item1, const void *item2)) {
    qsort(this->array_, this->size_, this->element_size_, compare);
}

_Bool array_list_try_save_to_file(const array_list* this, FILE* out) {
    fwrite(&this->element_size_, sizeof(this->element_size_), 1, out);
    fwrite(&this->size_, sizeof(this->size_), 1, out);
    fwrite(&this->capacity_, sizeof(this->capacity_), 1, out);
    fwrite(this->array_, this->element_size_, this->size_, out);
}

_Bool array_list_try_load_from_file(array_list* this, FILE* in) {
    fread(&this->element_size_, sizeof(this->element_size_), 1, in);
    fread(&this->size_, sizeof(this->size_), 1, in);
    fread(&this->capacity_, sizeof(this->capacity_), 1, in);

    array_list_resize(this, this->capacity_);
    fread(this->array_, this->element_size_, this->size_, in);
}

void array_list_export_to_txt(const array_list* this, FILE* out,const char* (*item_to_string)(const void* item)) {
    fprintf(out, "%d %d %d ", this->element_size_, this->size_, this->capacity_);
    char* item = this->array_;
    const char* end = this->array_ + this->size_ * this->element_size_;
    while (item < end) {
        const char* string = item_to_string(item);
        fprintf(out, "%s ", string);
        item += this->element_size_;
        free(string);
    }
}

array_list_iterator* array_list_get_begin_iterator(array_list* this, array_list_iterator* begin) {
    array_list_iterator_init(begin, this, 0);
    return begin;
}

array_list_iterator* array_list_get_end_iterator(array_list* this, array_list_iterator* end) {
    array_list_iterator_init(end, this, this->size_);
    return end;
}

void array_list_iterator_init(array_list_iterator* this, array_list *al, int index) {
    this->list_ = al;
    this->current_ = index;
}

void array_list_iterator_destroy(array_list_iterator* this) {
    this->list_ = NULL;
    this->current_ = -1;
}

_Bool array_list_iterator_is_equal(const array_list_iterator* this, const array_list_iterator* other) {
    return this->list_ == other->list_ && this->current_ == other->current_;
}

void array_list_iterator_move_next(array_list_iterator* this) {
    ++this->current_;
}

void array_list_iterator_move_get(const array_list_iterator* this, void* value) {
    array_list_try_get(this->list_, this->current_, value);
}

#undef ARRAY_LIST_DEFAULT_CAPACITY