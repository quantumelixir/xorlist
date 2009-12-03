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
    iter->dir = 1;

    return iter;
}

Iterator*
reverse_iter (List* list) {
    Iterator* iter = (Iterator *) malloc (sizeof (Iterator));
    iter->first = list->tail;
    iter->second = NULL;
    iter->dir = 0;

    return iter;
}

void
free_iter (Iterator* iter) {
    free (iter);
}

void
toggle_direction (Iterator* iter) {
    Node* temp = next(iter);

    // if forward make it a reverse iterator
    if (iter->dir) { 
        temp = iter->first;
        iter->first = iter->second;
        iter->second = temp;
    }
    else {
        temp = iter->second;
        iter->second = iter->first;
        iter->first = temp;
    }
}

/*
 * Return pointer to this node
 */
Node*
dereference (Iterator* iter) {
    return iter->dir ? iter->second : iter->first;
}

/*
 * Return pointer to "next" node according
 * to the direction of the iterator
 */
Node*
next (Iterator* iter) {
    if ((iter->dir == 1 && !iter->second)
            || (iter->dir == 0 && !iter->first))
        return NULL;
    return (Node *) (iter->dir ? ((int)iter->first) ^ ((int)iter->second->link) :
                       ((int)iter->first->link) ^ ((int)iter->second));
}

/*
 * Return pointer to "previous" node according
 * to the direction of the iterator
 */
Node*
prev (Iterator* iter) {
    return (Node *) (iter->dir ? ((int)iter->first) : ((int)iter->second));
}

/*
 * Move in the direction of the iterator
 * Returns true if the move was successful
 *
 * Moving past tail node and before head node
 * will yield unsuccessful move operations and
 * will not move the iterator beyond those limits
 */
int
move (Iterator* iter) {
    Node* temp = NULL;

    if (iter->dir) { 
        temp = iter->second;
        if (iter->second)
            iter->second = (Node *) (((int)iter->first) ^ ((int)iter->second->link));
        else
            return 0;
        iter->first = temp;
    }
    else {
        temp = iter->first;
        if (iter->first)
            iter->first = (Node *) (((int)iter->second) ^ ((int)iter->first->link));
        else
            return 0;
        iter->second = temp;
    }

    return 1;
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

    temp = dereference (iter);
    move(iter);
    while (temp) {
        free (temp);
        temp = dereference (iter);
        move(iter);
    }

    free_iter (iter);
}

void
traverse_list_in_dir (List* list,
        int dir, void (*callback)(void* data)) {
    Iterator* iter = dir ? forward_iter (list) : reverse_iter (list);

    callback (dereference (iter)->data);
    while (move (iter)) {
        callback (dereference (iter)->data);
    }

    free_iter (iter);
}

void
traverse_list_forward (List* list,
        void (*callback)(void* data)) {
    traverse_list_in_dir (list, 1, callback);
}

void
traverse_list_reverse (List* list,
        void (*callback)(void* data)) {
    traverse_list_in_dir (list, 0, callback);
}

/*
 * Inserts a node after the target node in the
 * direction of the iterator that points to it
 *
 * Modifies the iterator to point to the new node
 */
int
insert_node_next_to (List* list, Node* node, Iterator* target_iter) {
    Node* curr_node = dereference (target_iter);
    Node* next_node = next (target_iter);

    if (curr_node || next_node) {
        // manage links
        node->link = (Node *)(((int)curr_node) ^ ((int)next_node));
        if (curr_node)
            curr_node->link = (Node *)(((int)node) ^
                ((int)(curr_node ? curr_node->link : NULL) ^ (int)next_node));
        if (next_node)
            next_node->link = (Node *)(((int)node) ^
                ((int)(next_node ? next_node->link : NULL) ^ (int)curr_node));

        // update head
        if (!next_node && list->head == curr_node)
            list->head = node;
        // update tail
        if (list->tail == curr_node && !next_node)
            list->tail = node;

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

    return 1;
}

int
insert_node_before_head (List* list, Node* node) {
    Iterator* iter = forward_iter (list);
    toggle_direction (iter);
    insert_node_next_to (list, node, iter);
    free_iter (iter);

    return 1;
}

int
insert_node_after_tail (List* list, Node* node) {
    Iterator* iter = reverse_iter (list);
    toggle_direction (iter);
    insert_node_next_to (list, node, iter);
    free_iter (iter);

    return 1;
}

/*
 * Deletes the node pointed to by target_iter from the list
 */
int
delete_node_from_list (List* list, Iterator* target_iter) {
    Node* curr_node = dereference (target_iter);
    Node* next_node = next (target_iter);
    Node* prev_node = prev (target_iter);

    if (!curr_node)
        return 0;

    // middle of the list
    if (prev_node && next_node) { 
        // manage links
        if (prev_node)
            prev_node->link = (Node *) (((int)next_node) ^
                    (((int)prev_node->link) ^ ((int)curr_node)));
        if (next_node)
            next_node->link = (Node *) (((int)prev_node) ^
                    (((int)next_node->link) ^ ((int)curr_node)));
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

    return 1;
}
