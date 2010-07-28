#ifndef __NODE_H__
#define __NODE_H__

/*
 * The XOR Doubly Linked List requires
 * only one pointer per node
 */

typedef struct NodeTag Node;
struct NodeTag {
    void* data;
    Node* link;
};


/* create */
Node* create_node (void* data);

/* destroy */
void free_node (Node* node);

#endif /* __NODE_H__ */
