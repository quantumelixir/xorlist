#ifndef __LIST_H__
#define __LIST_H__

#include "bool.h"
#include "node.h"
#include "iterator.h"

typedef struct {
    Node* head;
    Node* tail;
    unsigned int size;
}List;

/* list creation */
List* create_empty_list ();

/* create iterators */
void forward_iter_init (List* list, Iterator* iter);
void reverse_iter_init (List* list, Iterator* iter);

/* list deletion */
void free_list (List* list);

/* list manipulation */
bool insert_node_next_to (List* list, Node* node, Iterator* iter);
bool insert_node_before_head (List* list, Node* node);
bool insert_node_after_tail (List* list, Node* node);
bool delete_node_from_list (List* list, Iterator* iter);

/* list traversal */
void traverse_list_forward (List* list, void (*callback)(void* data));
void traverse_list_reverse (List* list, void (*callback)(void* data));

#endif /* __LIST_H__ */
