#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H
#include <stdio.h>

typedef struct array_list {
    int element_size_;
    int size_;
    int capacity_;
    void* array_;
} array_list;

typedef struct array_list_iterator {
    array_list* list_;
    int current_;
} array_list_iterator;

void array_list_init(array_list* this, size_t element_size);
void array_list_destroy(array_list* this);
void array_list_clear(array_list* this);

int array_list_get_size(const array_list* this);
void array_list_add(array_list* this, const void* value);
_Bool array_list_try_get(const array_list* this, int index, void* value);
_Bool array_list_try_set(array_list* this, int index, const void* value);
void array_list_assign(array_list* this, const array_list* other);
void array_list_process_all(const array_list* this, void (*process_item)(void* item)); //pointer na funkciu
//cv10
void array_list_sort(array_list* this, int (*compare)(const void* item1,const void* item2));

//cv9
_Bool array_list_try_save_to_file(const array_list* this, FILE* out);
_Bool array_list_try_load_from_file(array_list* this, FILE* in);

void array_list_export_to_txt(const array_list* this, FILE* out,const char* (*item_to_string)(const void* item));
array_list_iterator* array_list_get_begin_iterator(array_list* this, array_list_iterator* begin);
array_list_iterator* array_list_get_end_iterator(array_list* this, array_list_iterator* end);

void array_list_iterator_init(array_list_iterator* this, array_list *al, int index);
void array_list_iterator_destroy(array_list_iterator* this);
_Bool array_list_iterator_is_equal(const array_list_iterator* this, const array_list_iterator* other);
void array_list_iterator_move_next(array_list_iterator* this);
void array_list_iterator_move_get(const array_list_iterator* this, void* value);



#endif //ARRAY_LIST_H