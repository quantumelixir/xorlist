#ifndef __NODE_H__
#define __NODE_H__

#include <stdio.h>

typedef struct Nd {
    void* data;
    unsigned int size;
    struct Nd* link;
}Node;

/* create */
Node* create_node (void* data, unsigned int n);

/* destroy */
void free_node (Node* node);

#endif /* __NODE_H__ */
