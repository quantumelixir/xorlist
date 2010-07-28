#include <malloc.h>
#include "node.h"
#include "iterator.h"
#include "list.h"

static Iterator temp_iter;

/* iterator initializers */
inline void
forward_iter_init (List* list, Iterator* iter) {
    iter->curr = list->head;
    iter->prev = NULL;
    iter->type = FORWARD;
}

inline void
reverse_iter_init (List* list, Iterator* iter) {
    iter->curr = list->tail;
    iter->prev = NULL;
    iter->type = REVERSE;
}

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
    free (list);
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
/*
 *
 *BEGIN
 *glist-style functions
 *
 *void
 *g_list_free (List *list)
 *{
 *    free_list (list);
 *}
 *
 *bool
 *g_list_append (List	*list, void* data)
 *{
 *    if (!list)
 *        return FALSE;
 *    return insert_node_after_tail (list, create_node (data));
 *}
 *
 *bool
 *g_list_prepend (List *list, void* data)
 *{
 *    if (!list)
 *        return FALSE;
 *    return insert_node_before_head (list, create_node (data));
 *}
 *
 * add list2 to the end of list1
 * i.e. list1 is modified while list2 is not.
 * The two lists must already exist although
 * their sizes maybe zero
 *
 * Returns True if the concatenation was performed
 *bool
 *g_list_concat (List *list1, List *list2)
 *{
 *    one of the lists doesn't exist
 *    if (!list1 || !list2)
 *        return FALSE;
 *
 *    list2 has no elements. Nothing to do.
 *    if (!list2->size)
 *        return FALSE;
 *
 *    list1 has no elements
 *    if (!list1->size) {
 *        list1->head = list2->head;
 *        list1->tail = list2->tail;
 *    }
 *    else {
 *        list1->tail->link = XOR (list1->tail->link, list2->head);
 *        list2->head->link = XOR (list1->tail, list2->head->link);
 *        list1->tail = list2->tail;
 *    }
 *
 *    list1->size += list2->size;
 *    return TRUE;
 *}
 *
 *bool
 *g_list_remove (List* list, const void* data)
 *{
 *    temp_iter.curr = list->head;
 *    temp_iter.prev = NULL;
 *    temp_iter.type = FORWARD;
 *
 *    Node* temp = DEREF (&temp_iter);
 *    while (move (&temp_iter)) {
 *        if (data == temp->data) {
 *            delete_node_from_list (list, temp_iter);
 *            return TRUE;
 *        }
 *        temp = DEREF (&temp_iter);
 *    }
 *
 *    return FALSE;
 *}
 *
 *bool
 *g_list_remove_all (List* list, const void* data)
 *{
 *    bool found = FALSE;
 *
 *    temp_iter.curr = list->head;
 *    temp_iter.prev = NULL;
 *    temp_iter.type = FORWARD;
 *
 *    Node* temp = DEREF (&temp_iter);
 *    while (move (&temp_iter)) {
 *        if (data == temp->data) {
 *            delete_node_from_list (list, temp_iter);
 *            found = TRUE;
 *        }
 *        temp = DEREF (&temp_iter);
 *    }
 *    return found;
 *}
 *
 *only remove the node from the list, don't delete
 *bool
 *g_list_remove_link (List* list, Iterator* iter)
 *{
 *    Node* curr_node = DEREF (iter);
 *    Node* next_node = NEXT (iter);
 *    Node* prev_node = PREV (iter);
 *
 *    if (!curr_node)
 *        return FALSE;
 *
 *    middle of the list
 *    if (prev_node && next_node) { 
 *        manage links
 *        if (prev_node)
 *            prev_node->link = XOR (next_node,
 *                    XOR (prev_node->link, curr_node));
 *        if (next_node)
 *            next_node->link = XOR (prev_node,
 *                    XOR (next_node->link, curr_node));
 *    }
 *    require head/tail update
 *    else {
 *        removed the last node
 *        if (!prev_node && !next_node)
 *            list->head = list->tail = NULL;
 *        else {
 *            update head
 *            if (iter->type == FORWARD) {
 *                list->head = next_node;
 *                next_node->link = XOR (next_node->link, curr_node);
 *            }
 *            update tail
 *            else if (iter->type == REVERSE) {
 *                list->tail = next_node;
 *                next_node->link = XOR (next_node->link, curr_node);
 *            }
 *        }
 *    }
 *
 *    node is now removed from the list
 *    curr_node->link = NULL;
 *
 *    list->size--;
 *
 *    return TRUE;
 *}
 *
 *bool
 *g_list_delete_link (List* list, Iterator* iter)
 *{
 *    delete_node_from_list (list, iter);
 *}
 *
 * shallow copy of the list
 * If the list elements contain pointers to data,
 * the pointers are copied but the data is not.
 *List*
 *g_list_copy (List *list)
 *{
 *    List* newlist = create_empty_list ();
 *
 *    temp_iter.curr = list->head;
 *    temp_iter.prev = NULL;
 *    temp_iter.type = FORWARD;
 *
 *    Node* temp = DEREF (&temp_iter);
 *    while (move (&temp_iter)) {
 *        insert_node_after_tail (newlist, create_node (temp->data));
 *        temp = DEREF (&temp_iter);
 *    }
 *
 *    return newlist;
 *}
 *
 *bool
 *g_list_reverse (List *list)
 *{
 *    if (!list) { 
 *        return FALSE;
 *    }
 *    temp = list->head;
 *    list->head = list->tail;
 *    list->tail = temp;
 *    return TRUE;
 *}
 *
 */
