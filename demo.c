#include <stdio.h>

#include "node.h"
#include "list.h"
#include "iterator.h"

// one million elements
#define N 100*100*100

void
print_int (void* i) {
    printf ("%5d\t", *(int *) i);
}

void
dumb_debug (List* list) {
    printf ("head:%d tail:%d size:%d\n", list->head ? *(int *)list->head->data : 0
                               , list->tail ? *(int *)list->tail->data : 0
                               , list->size);
}

int
main (int argc, char **argv) {

    int array[N];
    int i;

    List* list = create_empty_list ();
    Iterator iter;

    for ( i = 0; i < N; ++i ) {
        array[i] = i + 1;
    }

    printf ("Inserting elements now..\n");
    for ( i = 0; i < N; i += 2 ) {
        insert_node_before_head (list, create_node (array + i));
        insert_node_after_tail  (list, create_node (array + i + 1));
    }

    dumb_debug (list);

    Node* temp = NULL;
    while (scanf ("%d", &i)) {
        forward_iter_init (list, &iter);
        if (i < 0 || i >= list->size) { 
            break;
        }
        while (i--)
            move (&iter);
        temp = DEREF(&iter);
        i = *(int *)temp->data;
        printf ("%d\n", i);
    }

    printf ("Deleting elements now..\n");
    for ( i = 0; i < N; i += 2 ) {
        forward_iter_init (list, &iter);
        delete_node_from_list (list, &iter);
        reverse_iter_init (list, &iter);
        delete_node_from_list (list, &iter);
    }
    free_list (list);

    return 0;
}
