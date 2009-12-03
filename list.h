#ifndef __LIST_H__
#define __LIST_H__

#include "node.h"

typedef struct {
    Node* first;
    Node* second;
    int dir;
}Iterator;

typedef struct {
    Node* head;
    Node* tail;
}List;

/* create iterators */
Iterator* forward_iter (List* list);
Iterator* reverse_iter (List* list);

/* destory iterator */
void free_iter (Iterator* iter);

/* manipulate iterators */
void toggle_direction (Iterator* iter);
int move (Iterator* iter);
Node* rmove (Iterator* iter);

/* use iterators */
Node* dereference (Iterator* iter);
Node* next (Iterator* iter);
Node* prev (Iterator* iter);

/* list creation */
List* create_empty_list ();

/* list deletion */
void free_list (List* list);

/* list manipulation */
int insert_node_next_to (List* list, Node* node, Iterator* target_iter);
int insert_node_before_head (List* list, Node* node);
int insert_node_after_tail (List* list, Node* node);
int delete_node (List* list, Iterator* target_iter);

/* list traversal */
void traverse_list_in_dir (List* list, int dir,
        void (*callback)(void* data));
void traverse_list_forward (List* list,
        void (*callback)(void* data));
void traverse_list_reverse (List* list,
        void (*callback)(void* data));

#endif /* __LIST_H__ */
