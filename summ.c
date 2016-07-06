#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "eqtypes.h"
#include "common.h"
#include "summ.h"

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
  
  if(eq_children_count(node) == 0)
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
        continue;
      }

      prev = (struct eq_node **)&child->next;
      child = child->next;
    }
  }
}

void eq_delete_zero(struct eq_node *summ)
{
  struct eq_leaf **prev = &summ->first_child;
  struct eq_leaf *leaf = summ->first_child;
  
  while(leaf != NULL) {
    if(leaf->type == EQ_NUMBER && leaf->value == 0)
      *prev = eq_delete(leaf);
    else 
      prev = &leaf->next;
    
    leaf = *prev;
  }
}

/*
 * Calculate summ. Node type must be EQ_SUMM
 */
void eq_calculate_summ(struct eq_node **node)
{
  struct eq_leaf **num = (struct eq_leaf **)&(*node)->first_child;
  struct eq_leaf *leaf;
  struct eq_leaf **prev;
  
  num = (struct eq_leaf **)&(*node)->first_child;
  
  while(*num != NULL && (*num)->type != EQ_NUMBER)
    num = (struct eq_leaf **)&(*num)->next;
  
  if (*num == NULL)
    goto ret;
  
  prev = (struct eq_leaf **)&(*num)->next;
  leaf = (*num)->next;
  double val = (*num)->value * (*num)->sign;
  while(leaf != NULL) {
    if(leaf->type == EQ_NUMBER) {
      val += leaf->value * leaf->sign;
      *prev = leaf->next;
      eq_delete(leaf);
    } else 
      prev = (struct eq_leaf **)&leaf->next;
    
    leaf = *prev;
  }
  (*num)->value = fabs(val);
  (*num)->sign = 1;
  if(val < 0)
    (*num)->sign = -1;
ret:
  return;
}

/*
 * Transform summ.
 */
void eq_transform_summ(void **summ)
{
  struct eq_node *child = ((struct eq_node *)(*summ))->first_child;
  if(child == NULL) {
    void *next = eq_delete(*summ);
    *summ = eq_leaf_new(EQ_NUMBER, 1, "", 0);
    (*(struct eq_node **)summ)->next = next; 
    goto ret;
  }
  
  if(eq_children_count(*summ) == 1) {
    void *res = ((struct eq_node *)(*summ))->first_child;
    ((struct eq_node *)(*summ))->first_child = NULL;
    ((struct eq_node *)res)->next = eq_delete(*summ);
    *summ = res;
    goto ret;
  } else {
    eq_combine_summ(*summ);
    eq_delete_zero(*summ);
  }
  
ret:
  return;
}

/*
 * Get pointer to first summ number
 */
struct eq_leaf *get_summ_number(struct eq_node *summ)
{
  if(summ->type != EQ_SUMM)
    return NULL;
  
  struct eq_leaf *child = summ->first_child;
  while(child != NULL) {
    if(child->type == EQ_NUMBER)
      break;
    child = child->next;
  }
  
  return child;
}