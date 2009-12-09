#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#include "bool.h"
#include "node.h"

/* List iterator requires pointers to two
 * nodes (as XOR is a binary operation)
 */
typedef enum {
    REVERSE, FORWARD
}IteratorType;

typedef struct _Iterator Iterator;
struct _Iterator {
    Node* curr;
    Node* prev;
    IteratorType type;
};

/* everything depends on this being able to work! */
#define XOR(A,B) ((Node *)(((int)(A)) ^ ((int)(B))))

/* use iterators */
#define DEREF(A) ((A)->curr)
#define PREV(A)  ((A)->prev)
#define NEXT(A)  ((A)->curr ? XOR ((A)->prev, (A)->curr->link) : NULL)

/* manipulate iterators */
void free_iter (Iterator* iter);
bool toggle_direction (Iterator* iter);
bool move (Iterator* iter);

#endif /* __ITERATOR_H__ */
