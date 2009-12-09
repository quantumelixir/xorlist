#include <malloc.h>

#include "node.h"
#include "iterator.h"
#include "list.h"

List*
create_empty_list () {
    List* list = (List *) malloc (sizeof (List));
    list->head = list->tail = NULL;
    list->size = 0;
    return list;
}

void
free_list (List* list) {
    Iterator* iter = forward_iter (list);

    Node* temp = DEREF (iter);
    while (move (iter)) {
        free_node (temp);
        temp = DEREF (iter);
    }

    free_iter (iter);
}

void
traverse_list_forward (List* list, void (*callback)(void* data)) {
    Iterator* iter = forward_iter (list);

    Node* temp = DEREF (iter);
    while (move (iter)) {
        callback (temp->data);
        temp = DEREF (iter);
    }
}

void
traverse_list_reverse (List* list, void (*callback)(void* data)) {
    Iterator* iter = reverse_iter (list);

    Node* temp = DEREF (iter);
    while (move (iter)) {
        callback (temp->data);
        temp = DEREF (iter);
    }
}

/*
 * Inserts a node after the target node in the
 * direction of the iterator that points to it
 *
 * Modifies the iterator to point to the new node
 */
bool
insert_node_next_to (List* list, Node* node, Iterator* iter) {
    Node* curr_node = DEREF (iter);
    Node* next_node = NEXT (iter);

    // if not the first node of the list
    if (curr_node || next_node) {
        // manage links
        node->link = XOR (curr_node, next_node);
        if (curr_node)
            curr_node->link = XOR (node, XOR (curr_node->link, next_node));
        if (next_node)
            next_node->link = XOR (node, XOR (next_node->link, curr_node));

        // move the list head/tail but not both at the same time!
        if (!next_node) { 
            if (list->head == curr_node)
                list->head = node;
            else if (list->tail == curr_node)
                list->tail = node;
        }
    }
    // first node of the list
    else {
        node->link = NULL;
        list->head = list->tail = node;
    }

    list->size++;

    // Should we make the iter point to new node?
    //iter->curr = node;
    //iter->prev = NULL;

    return TRUE;
}

bool
insert_node_before_head (List* list, Node* node) {
    Iterator* iter = forward_iter (list);
    toggle_direction (iter);
    insert_node_next_to (list, node, iter);
    free (iter);

    return TRUE;
}

bool
insert_node_after_tail (List* list, Node* node) {
    Iterator* iter = reverse_iter (list);
    toggle_direction (iter);
    insert_node_next_to (list, node, iter);
    free (iter);

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
            if (list->head == curr_node)
                list->head = next_node;
            // update tail
            if (list->tail == curr_node)
                list->tail = prev_node;
        }
    }

    free_node (curr_node);

    list->size--;

    // iter now points to an invalid location
    // should we do something about it?

    return TRUE;
}
