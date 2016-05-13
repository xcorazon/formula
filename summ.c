#include "eqtypes.h"
#include "common.h"
#include "summ.h"
#include <stdlib.h>
#include <stdbool.h>

/*
 * Add to node another node.
 */
struct eq_node *eq_add(void **node1, void **node2)
{
  struct eq_node *summ;
  
  if(((struct eq_node *)(*node1))->type == EQ_SUMM) {
    summ = *node1;
    eq_move_children(summ, (struct eq_node *)(*node2));
    eq_delete(*node2);
    *node2 = NULL;
  }
  else if(((struct eq_node *)(*node2))->type == EQ_SUMM) {
    summ = *node2;
    eq_move_children(summ, (struct eq_node *)(*node1));
    eq_delete(*node1);
    *node1 = NULL;
  }
  else {
    summ = eq_node_new(EQ_SUMM, 1);
    summ->first_child = *node1;
    ((struct eq_node *)(*node1))->next = *node2;
  }
  
  return summ;
}

/*
 * If one of children is summ - his children moves to current node.
 */
void eq_combine_summ(struct eq_node *node)
{
  if(node == NULL || node->type == EQ_SYMBOL || node->type == EQ_NUMBER)
    return;
  
  struct eq_node *child = node->first_child;
  struct eq_node **prev = (struct eq_node **)(&node->first_child);
  
  while(child != NULL) {
    eq_combine_summ(child);
    child = child->next;
  }
  
  child = node->first_child;
  
  if(node->type == EQ_SUMM) {
    while(child != NULL) {
      
      if(child->type == EQ_SUMM) {
        eq_move_children(node, child);
        *prev = child->next;
        eq_delete(child);
        child = *prev;
      } else
        prev = (struct eq_node **)&child->next;
      
      child = child->next;
    }
  }
}