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
    Node* four = create_node ("four", 5);

    List* list = create_empty_list ();

    insert_node_after_tail (list, one);
    traverse_list_forward (list, print_string);
    printf ("-----\n");

    insert_node_after_tail (list, two);
    traverse_list_forward (list, print_string);
    printf ("-----\n");

    insert_node_before_head (list, three);
    traverse_list_forward (list, print_string);
    printf ("-----\n");

    insert_node_after_tail (list, four);
    traverse_list_forward (list, print_string);
    printf ("-----\n");

    free_list (list);

    return 0;
}
