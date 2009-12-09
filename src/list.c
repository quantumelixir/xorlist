#include <malloc.h>
#include "node.h"
#include "iterator.h"
#include "list.h"

static Iterator temp_iter;

List*
create_empty_list () {
    List* list = (List *) malloc (sizeof (List));
    list->head = list->tail = NULL;
    list->size = 0;
    return list;
}

void
free_list (List* list) {
    temp_iter.curr = list->head;
    temp_iter.prev = NULL;

    Node* temp = DEREF (&temp_iter);
    while (move (&temp_iter)) {
        free_node (temp);
        temp = DEREF (&temp_iter);
    }
}

void
traverse_list_forward (List* list, void (*callback)(void* data)) {
    temp_iter.curr = list->head;
    temp_iter.prev = NULL;
    temp_iter.type = FORWARD;

    Node* temp = DEREF (&temp_iter);
    while (move (&temp_iter)) {
        callback (temp->data);
        temp = DEREF (&temp_iter);
    }
}

void
traverse_list_reverse (List* list, void (*callback)(void* data)) {
    temp_iter.curr = list->tail;
    temp_iter.prev = NULL;
    temp_iter.type = REVERSE;

    Node* temp = DEREF (&temp_iter);
    while (move (&temp_iter)) {
        callback (temp->data);
        temp = DEREF (&temp_iter);
    }
}

/*
 * Inserts a node after the target node in the
 * direction of the iterator that points to it
 */
bool
insert_node_next_to (List* list, Node* node, Iterator* iter) {
    Node* curr_node;
    Node* next_node;

    // if not the first node of the list
    if (list->size) {
        curr_node = DEREF (iter);
        next_node = NEXT (iter);

        // manage links
        node->link = XOR (curr_node, next_node);
        if (curr_node)
            curr_node->link = XOR (node, XOR (curr_node->link, next_node));
        if (next_node)
            next_node->link = XOR (node, XOR (next_node->link, curr_node));

        // update the list head/tail but not both at the same time!
        if (!next_node) { 
            if (iter->type == REVERSE)
                list->head = node;
            else if (iter->type == FORWARD)
                list->tail = node;
        }
    }
    // first node of the list
    else {
        node->link = NULL;
        list->head = list->tail = node;
    }

    list->size++;

    return TRUE;
}

bool
insert_node_before_head (List* list, Node* node) {
    temp_iter.curr = list->head;
    temp_iter.prev = NULL;
    temp_iter.type = FORWARD;
    toggle_direction (&temp_iter);
    insert_node_next_to (list, node, &temp_iter);

    return TRUE;
}

bool
insert_node_after_tail (List* list, Node* node) {
    temp_iter.curr = list->tail;
    temp_iter.prev = NULL;
    temp_iter.type = REVERSE;
    toggle_direction (&temp_iter);
    insert_node_next_to (list, node, &temp_iter);

    return TRUE;
}

/*
 * Deletes the node pointed to by target_iter from the list
 */
bool
delete_node_from_list (List* list, Iterator* iter) {
    Node* curr_node = DEREF (iter);
    Node* next_node = NEXT (iter);
    Node* prev_node = PREV (iter);

    if (!curr_node)
        return FALSE;

    // middle of the list
    if (prev_node && next_node) { 
        // manage links
        if (prev_node)
            prev_node->link = XOR (next_node,
                    XOR (prev_node->link, curr_node));
        if (next_node)
            next_node->link = XOR (prev_node,
                    XOR (next_node->link, curr_node));
    }
    // require head/tail update
    else {
        // removed the last node
        if (!prev_node && !next_node)
            list->head = list->tail = NULL;
        else {
            // update head
            if (iter->type == FORWARD) {
                list->head = next_node;
                next_node->link = XOR (next_node->link, curr_node);
            }
            // update tail
            else if (iter->type == REVERSE) {
                list->tail = next_node;
                next_node->link = XOR (next_node->link, curr_node);
            }
        }
    }

    free_node (curr_node);

    list->size--;

    return TRUE;
}
