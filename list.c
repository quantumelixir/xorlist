#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "list.h"

/*
 * While normal doubly linked lists require only
 * a single pointer to access its successor/predecessor,
 * an XOR linked lists requires two which means most
 * operations like insert, delete, etc. require two pointers
 *
 * => Use the Iterator structure, which stores two Node pointers
 *    at a time and can be moved both forward and backward
 *
 *    iter->dir = 1 => Forward Iterator
 *      iter->second gives the location pointed to by iter
 *
 *    iter->dir = 0 => Reverse Iterator
 *      iter->first gives the location pointed to by iter
 */
Iterator*
forward_iter (List* list) {
    Iterator* iter = (Iterator *) malloc (sizeof (Iterator));
    iter->first = NULL;
    iter->second = list->head;
    iter->dir = FORWARD;

    return iter;
}

Iterator*
reverse_iter (List* list) {
    Iterator* iter = (Iterator *) malloc (sizeof (Iterator));
    iter->first = list->tail;
    iter->second = NULL;
    iter->dir = REVERSE;

    return iter;
}

void
free_iter (Iterator* iter) {
    free (iter);
}

void
toggle_direction (Iterator* iter) {
    Node* temp = NEXT(iter);

    // if forward make it a reverse iterator
    if (iter->dir) { 
        iter->first = iter->second;
        iter->second = temp;
        iter->dir = REVERSE;
    }
    else {
        iter->second = iter->first;
        iter->first = temp;
        iter->dir = FORWARD;
    }
}

/*
 * Move in the direction of the iterator
 * Returns true if the move was successful
 *
 * Moving past tail node and before head node
 * will yield unsuccessful move operations and
 * will not move the iterator beyond those limits
 */
bool
move (Iterator* iter) {
    Node* temp = NULL;

    if (iter->dir) { 
        temp = iter->second;
        if (iter->second)
            iter->second = XOR(iter->first, iter->second->link);
        else
            return FALSE;
        iter->first = temp;
    }
    else {
        temp = iter->first;
        if (iter->first)
            iter->second = XOR(iter->second, iter->first->link);
        else
            return FALSE;
        iter->second = temp;
    }

    return TRUE;
}

/*
 * Move in a direction opposite to that of the
 * iterator returning a pointer to the new node
 * that the iterator points to
 */
Node*
rmove (Iterator* iter) {
    toggle_direction (iter);
    move (iter);
    toggle_direction (iter);
    return iter->dir ? iter->second : iter->first;
}

List*
create_empty_list () {
    List* list = (List *) malloc (sizeof (List));
    list->head = list->tail = NULL;
    return list;
}

void
free_list (List* list) {
    Iterator* iter = forward_iter (list);
    Node* temp = NULL;

    temp = DEREF(iter);
    move(iter);
    while (temp) {
        free (temp);
        temp = DEREF(iter);
        move(iter);
    }

    free_iter (iter);
}

void
traverse_list_in_dir (List* list,
        iter_type dir, void (*callback)(void* data)) {
    Iterator* iter = dir ? forward_iter (list) : reverse_iter (list);
    Node* temp = DEREF(iter);

    while (move (iter)) {
        callback (temp->data);
        temp = DEREF(iter);
    }

    free_iter (iter);
}

void
traverse_list_forward (List* list,
        void (*callback)(void* data)) {
    traverse_list_in_dir (list, FORWARD, callback);
}

void
traverse_list_reverse (List* list,
        void (*callback)(void* data)) {
    traverse_list_in_dir (list, REVERSE, callback);
}

/*
 * Inserts a node after the target node in the
 * direction of the iterator that points to it
 *
 * Modifies the iterator to point to the new node
 */
bool
insert_node_next_to (List* list, Node* node, Iterator* target_iter) {
    Node* curr_node = DEREF(target_iter);
    Node* next_node = NEXT(target_iter);

    if (curr_node || next_node) {
        // manage links
        node->link = XOR(curr_node, next_node);
        if (curr_node)
            curr_node->link = XOR(node, XOR(curr_node->link, next_node));
        if (next_node)
            next_node->link = XOR(node, XOR(next_node->link, curr_node));

        // manage list head/tail
        if (!next_node) { 
            if (list->head == curr_node && target_iter->dir == REVERSE)
                list->head = node;
            else if (list->tail == curr_node && target_iter->dir == FORWARD)
                list->tail = node;
        }

        // Make the iter point to new node
        target_iter->first = target_iter->dir ? curr_node : node ;
        target_iter->second = target_iter->dir ? node : curr_node ;
    }
    // first node of the list
    else {
        node->link = NULL;
        list->head = list->tail = node;

        // Make the iter point to new node
        target_iter->first = target_iter->dir ? NULL : list->tail ;
        target_iter->second = target_iter->dir ? list->head : NULL ;
    }

    return TRUE;
}

bool
insert_node_before_head (List* list, Node* node) {
    Iterator* iter = forward_iter (list);
    toggle_direction (iter);
    insert_node_next_to (list, node, iter);
    free_iter (iter);

    return TRUE;
}

bool
insert_node_after_tail (List* list, Node* node) {
    Iterator* iter = reverse_iter (list);
    toggle_direction (iter);
    insert_node_next_to (list, node, iter);
    free_iter (iter);

    return TRUE;
}

/*
 * Deletes the node pointed to by target_iter from the list
 */
bool
delete_node_from_list (List* list, Iterator* target_iter) {
    Node* curr_node = DEREF(target_iter);
    Node* next_node = NEXT(target_iter);
    Node* prev_node = PREV(target_iter);

    if (!curr_node)
        return FALSE;

    // middle of the list
    if (prev_node && next_node) { 
        // manage links
        if (prev_node)
            prev_node->link = XOR(next_node,
                    XOR(prev_node->link, curr_node));
        if (next_node)
            next_node->link = XOR(prev_node,
                    XOR(next_node->link, curr_node));
    }
    // require head/tail update
    else {
        // removed the last node
        if (!prev_node && !next_node)
            list->head = list->tail = NULL;
        else {
            // update head
            if (!prev_node)
                list->head = next_node;
            // update tail
            if (!next_node)
                list->tail = prev_node;
        }
    }

    free_node (curr_node);

    return TRUE;
}
