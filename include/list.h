#ifndef __LIST_H__
#define __LIST_H__

#include "bool.h"
#include "node.h"
#include "iterator.h"

typedef struct {
    Node* head;
    Node* tail;
    unsigned int size;
}List;

/* list creation */
List* create_empty_list ();

/* initialize list iterators */
void forward_iter_init (List* list, Iterator* iter);
void reverse_iter_init (List* list, Iterator* iter);

/* list deletion */
void free_list (List* list);

/* list manipulation */
bool insert_node_next_to (List* list, Node* node, Iterator* iter);
bool insert_node_before_head (List* list, Node* node);
bool insert_node_after_tail (List* list, Node* node);
bool delete_node_from_list (List* list, Iterator* iter);

/* list traversal */
void traverse_list_forward (List* list, void (*callback)(void* data));
void traverse_list_reverse (List* list, void (*callback)(void* data));

/*
 *
 *[> glist-style functions <]
 *
 *List*
 *g_list_insert (List	*list,
 *        void*	 data,
 *        gint	 position)
 *{
 *}
 *
 *[> counts from 0 <]
 *void*
 *g_list_nth (List *list, int n)
 *{
 *    temp_iter.curr = list->head;
 *    temp_iter.prev = NULL;
 *    temp_iter.type = FORWARD;
 *
 *    Node* temp = DEREF (&temp_iter);
 *    while ((n-- > 0) && move (&temp_iter)) {
 *        temp = DEREF (&temp_iter);
 *    }
 *    return temp_iter.curr ? temp_iter.curr->data : NULL;
 *}
 *
 *void*
 *g_list_nth_from (List *list, int n)
 *{
 *    temp_iter.curr = list->head;
 *    temp_iter.prev = NULL;
 *    temp_iter.type = FORWARD;
 *
 *    Node* temp = DEREF (&temp_iter);
 *    while ((n-- > 0) && move (&temp_iter)) {
 *        temp = DEREF (&temp_iter);
 *    }
 *    return temp_iter.curr ? temp_iter.curr->data : NULL;
 *}
 *
 *void*
 *g_list_nth_data (List     *list,
 *        guint      n)
 *{
 *}
 *
 *List*
 *g_list_find (List         *list,
 *        const void *  data)
 *{
 *}
 *
 *List*
 *g_list_find_custom (List         *list,
 *            const void *  data,
 *            GCompareFunc   func)
 *{
 *}
 *
 *
 *gint
 *g_list_position (List *list,
 *        List *llink)
 *{
 *}
 *
 *gint
 *g_list_index (List         *list,
 *        const void *  data)
 *{
 *}
 *
 *List*
 *g_list_last (List *list)
 *{
 *}
 *
 *List*
 *g_list_first (List *list)
 *{
 *}
 *
 *guint
 *g_list_length (List *list)
 *{
 *}
 *
 *void
 *g_list_foreach (List	 *list,
 *        GFunc	  func,
 *        void*  user_data)
 *{
 *}
 *
 *static List*
 *g_list_insert_sorted_real (List    *list,
 *            void*  data,
 *            GFunc     func,
 *            void*  user_data)
 *{
 *List *tmp_list = list;
 *List *new_list;
 *gint cmp;
 *
 *g_return_val_if_fail (func != NULL, list);
 *
 *if (!list) 
 *    {
 *    new_list = _g_list_alloc0 ();
 *    new_list->data = data;
 *    return new_list;
 *    }
 *
 *cmp = ((GCompareDataFunc) func) (data, tmp_list->data, user_data);
 *
 *while ((tmp_list->next) && (cmp > 0))
 *    {
 *    tmp_list = tmp_list->next;
 *
 *    cmp = ((GCompareDataFunc) func) (data, tmp_list->data, user_data);
 *    }
 *
 *new_list = _g_list_alloc0 ();
 *new_list->data = data;
 *
 *if ((!tmp_list->next) && (cmp > 0))
 *    {
 *    tmp_list->next = new_list;
 *    new_list->prev = tmp_list;
 *    return list;
 *    }
 *
 *if (tmp_list->prev)
 *    {
 *    tmp_list->prev->next = new_list;
 *    new_list->prev = tmp_list->prev;
 *    }
 *new_list->next = tmp_list;
 *tmp_list->prev = new_list;
 *
 *if (tmp_list == list)
 *    return new_list;
 *else
 *    return list;
 *}
 *
 *List*
 *g_list_insert_sorted (List        *list,
 *            void*      data,
 *            GCompareFunc  func)
 *{
 *}
 *
 *List*
 *g_list_insert_sorted_with_data (List            *list,
 *                void*          data,
 *                GCompareDataFunc  func,
 *                void*          user_data)
 *{
 *}
 *
 *static List *
 *g_list_sort_merge (List     *l1, 
 *        List     *l2,
 *        GFunc     compare_func,
 *        void*  user_data)
 *{
 *List list, *l, *lprev;
 *gint cmp;
 *
 *l = &list; 
 *lprev = NULL;
 *
 *while (l1 && l2)
 *    {
 *    cmp = ((GCompareDataFunc) compare_func) (l1->data, l2->data, user_data);
 *
 *    if (cmp <= 0)
 *        {
 *    l->next = l1;
 *    l1 = l1->next;
 *        } 
 *    else 
 *    {
 *    l->next = l2;
 *    l2 = l2->next;
 *        }
 *    l = l->next;
 *    l->prev = lprev; 
 *    lprev = l;
 *    }
 *l->next = l1 ? l1 : l2;
 *l->next->prev = l;
 *
 *return list.next;
 *}
 *
 *static List* 
 *g_list_sort_real (List    *list,
 *        GFunc     compare_func,
 *        void*  user_data)
 *{
 *List *l1, *l2;
 *
 *if (!list) 
 *    return NULL;
 *if (!list->next) 
 *    return list;
 *
 *l1 = list; 
 *l2 = list->next;
 *
 *while ((l2 = l2->next) != NULL)
 *    {
 *    if ((l2 = l2->next) == NULL) 
 *    break;
 *    l1 = l1->next;
 *    }
 *l2 = l1->next; 
 *l1->next = NULL; 
 *
 *return g_list_sort_merge (g_list_sort_real (list, compare_func, user_data),
 *                g_list_sort_real (l2, compare_func, user_data),
 *                compare_func,
 *                user_data);
 *}
 *
 *List *
 *g_list_sort (List        *list,
 *        GCompareFunc  compare_func)
 *{
 *                
 *}
 *
 *List *
 *g_list_sort_with_data (List            *list,
 *            GCompareDataFunc  compare_func,
 *            void*          user_data)
 *{
 *}
 *
 */

#endif /* __LIST_H__ */
