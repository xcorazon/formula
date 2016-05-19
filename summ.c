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
  
  summ = eq_node_new(EQ_SUMM, 1);
  summ->first_child = *node1;
  ((struct eq_node *)(*node1))->next = *node2;
  eq_combine_summ(summ);

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

/*
 * Calculate summ. Node type must be EQ_SUMM
 */
void eq_calculate_summ(struct eq_node *node)
{
  struct eq_leaf *num = node->first_child;
  struct eq_leaf *leaf;
  struct eq_leaf **prev;
  
  while(num != NULL && num->type != EQ_NUMBER)
    num = num->next;
  
  if (num == NULL)
    goto ret;
  
  prev = (struct eq_leaf **)&num->next;
  leaf = num->next;
  while(leaf != NULL) {
    if(leaf->type == EQ_NUMBER) {
      num->value += leaf->value;
      *prev = leaf->next;
      eq_delete(leaf);
    } else 
      prev = (struct eq_leaf **)&leaf->next;
    
    leaf = *prev;
  }
ret:
  return;
}