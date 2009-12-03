#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "node.h"

Node*
create_node (void* data, unsigned int n) {
    Node* newnode = (Node *) malloc (sizeof (Node));

    newnode->data = (void *) malloc (n);
    memcpy (newnode->data, data, n);
    newnode->size = n;

    newnode->link = NULL;

    return newnode;
}

void
free_node (Node* node) {
    free (node->data);
    free (node);
}
