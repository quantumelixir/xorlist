#ifndef __LIST_H__
#define __LIST_H__

#include "node.h"

// Boolean type
typedef enum {
    FALSE = 0, TRUE = 1
}bool;

// Iterator type
typedef enum {
    REVERSE = 0, FORWARD = 1
}iter_type;

// List iterator requires pointers to two
// nodes (as XOR is a binary operation) and
// have a direction
typedef struct {
    Node* first;
    Node* second;
    iter_type dir;
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
bool move (Iterator* iter);
Node* rmove (Iterator* iter);

/* use iterators */
#define DEREF(A) ((A)->dir == FORWARD ? (A)->second : (A)->first)
#define XOR(A,B) ((Node *)(((int)(A)) ^ ((int)(B))))
#define PREV(A) ((A)->dir == FORWARD ? (A)->first : (A)->second)
#define NEXT(A) ((((A)->dir == FORWARD && !(A)->second) ||                  \
                ((A)->dir == REVERSE && !(A)->first)) ? NULL :              \
                ((A)->dir == FORWARD ? XOR((A)->first, (A)->second->link) : \
                XOR((A)->first->link, (A)->second)))

/* list creation */
List* create_empty_list ();

/* list deletion */
void free_list (List* list);

/* list manipulation */
bool insert_node_next_to (List* list, Node* node, Iterator* target_iter);
bool insert_node_before_head (List* list, Node* node);
bool insert_node_after_tail (List* list, Node* node);
bool delete_node (List* list, Iterator* target_iter);

/* list traversal */
void traverse_list_in_dir (List* list, iter_type dir,
        void (*callback)(void* data));
void traverse_list_forward (List* list,
        void (*callback)(void* data));
void traverse_list_reverse (List* list,
        void (*callback)(void* data));

#endif /* __LIST_H__ */
