#include <stdio.h>

#include "node.h"
#include "list.h"

void
print_string (void* s) {
    printf ("%s\n", (char *) s);
}

int
main (int argc, char **argv) {
    Node* one = create_node ("one", 4);
    Node* two = create_node ("two", 4);
    Node* three = create_node ("three", 6);

    List* list = create_empty_list ();
    insert_node_before_head (list, three);
    insert_node_before_head (list, two);
    insert_node_before_head (list, one);
    traverse_list_forward (list, print_string);

    free_list (list);

    return 0;
}
