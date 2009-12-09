#include <malloc.h>
#include "list.h"
#include "iterator.h"
#include "node.h"

/*
 * While normal doubly linked lists require only
 * a single pointer to access its successor/predecessor,
 * an XOR linked lists requires two which means most
 * operations like insert, delete, etc. require two pointers
 *
 * => Use the Iterator structure, which stores two Node pointers
 * => Two kinds: Forward and Reverse Iterator
 *      Forward Iterator
 *          curr is always "ahead" of prev, while traversing the
 *          list forwards
 *      Reverse Iterator
 *          curr is always "ahead" of prev, while traversing the
 *          list backwards
 *    List helper functions are oblivous to what kind of an
 *    iterator they are operating with.
 */

Iterator*
forward_iter (List* list) {
    Iterator* iter = (Iterator *) malloc (sizeof (Iterator));
    iter->curr = list->head;
    iter->prev = NULL;

    return iter;
}

Iterator*
reverse_iter (List* list) {
    Iterator* iter = (Iterator *) malloc (sizeof (Iterator));
    iter->curr = list->tail;
    iter->prev = NULL;

    return iter;
}

void
free_iter (Iterator* iter) {
    free (iter);
}

/*
 * Can toggle the direction of iterator only
 * if the iterator points to a non-null node
 *
 * Returns TRUE if the toggle was successful
 */
inline bool
toggle_direction (Iterator* iter) {
    if (iter->curr)
        iter->prev = NEXT (iter);
    else
        return FALSE;
    return TRUE;
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
    Node* temp = iter->curr;

    if (iter->curr)
        iter->curr = NEXT (iter);
    else
        return FALSE;
    iter->prev = temp;

    return TRUE;
}
