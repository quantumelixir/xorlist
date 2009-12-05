#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "node.h"

Node*
create_node (void* data) {
    Node* newnode = (Node *) malloc (sizeof (Node));

    newnode->data = data;
    newnode->link = NULL;

    return newnode;
}

void
free_node (Node* node) {
    free (node);
}
