#include <stdlib.h>
#include <stdbool.h>
#include "reciprocal.h"
#include "mul.h"
#include "common.h"

void eq_combine_recip(struct eq_node *mul)
{
  if(eq_is_leaf(mul))
    goto ret;
  
  struct eq_node *child = mul->first_child;
  while(child != NULL){
    eq_combine_recip(child);
    child = child->next;
  }
  
  if(mul->type != EQ_MUL)
    goto ret;
  
  struct eq_node *recip = mul->first_child;
  while(recip != NULL) {
    if(recip->type == EQ_RECIPROCAL)
      break;
    recip = recip->next;
  }
  
  if(recip == NULL)
    goto ret;
  
  child = recip->next;
  struct eq_node **element = &recip->next;
  while(*element != NULL) {
    if((*element)->type == EQ_RECIPROCAL) {
      eq_move_children(recip, *element);
      *element = eq_delete(*element);
      continue;
    }
    element = (struct eq_node **)&(*element)->next;
  }
  
ret:
  return;
}

void eq_unwrap_recip(struct eq_node **recip)
{
  if(eq_is_leaf(*recip))
    goto ret;
  
  void **prev = &(*recip)->first_child;
  while(*prev != NULL){
    eq_unwrap_recip((struct eq_node **)prev);
    prev = &((struct eq_node *)*prev)->next;
  }
  
  if((*recip)->type != EQ_RECIPROCAL)
    goto ret;
  
  if(eq_children_count(*recip) <= 1)
    goto ret;
  
  struct eq_node *mul = eq_node_new(EQ_MUL, (*recip)->sign);
  struct eq_node **multiplier = (struct eq_node **)&mul->first_child;
  prev = &((*recip)->first_child);
  struct eq_node *child = *prev;
  
  while(*prev != NULL) {
    struct eq_node *rec = eq_node_new(EQ_RECIPROCAL, 1);
    rec->first_child = *prev;
    prev = &((struct eq_node *)*prev)->next;
    child->next = NULL;
    child = *prev;
    *multiplier = rec;
    multiplier = (struct eq_node **)&(rec->next);
  }
  
  *multiplier = NULL;
  (*recip)->first_child = NULL;
  mul->next = eq_delete(*recip);
  *recip = mul;
  
ret:
  return;
}

void eq_calculate_reciprocal(struct eq_node **node)
{
  eq_calculate_mul(node);
  
  if(eq_children_count(*node) == 1) {
    struct eq_leaf *child = (*node)->first_child;
    
    if(child->type == EQ_NUMBER) {
      child->value = 1 / child->value;
      child->sign *= (*node)->sign;
      (*node)->first_child = NULL;
      child->next = eq_delete(*node);
      *node = child;
    }
  }
  return;
}


void eq_transform_reciprocal(void **rec)
{
  struct eq_node *child;
  int count = eq_children_count(*rec);
  
  if(count == 0) {
    struct eq_leaf *num = eq_leaf_new(EQ_NUMBER, ((struct eq_node *)*rec)->sign, NULL, 1.0);
    num->next = eq_delete(*rec);
    *rec = num;
    goto ret;
  }
  
  if(count == 1) {
    child = ((struct eq_node *)*rec)->first_child;
    
    if(child->type == EQ_RECIPROCAL) {
        ((struct eq_node *)(*rec))->type = EQ_MUL;
        eq_move_children(*rec, child);
        ((struct eq_node *)(*rec))->first_child = eq_delete(child);
        
    } else if(child->type == EQ_MUL) {
        eq_move_children(*rec, child);
        ((struct eq_node *)(*rec))->first_child = eq_delete(child);
    }
    goto ret;
  }
  
  /* count > 1 */
  struct eq_node **prev = (struct eq_node **)&((struct eq_node *)*rec)->first_child;
  struct eq_node *mul = eq_node_new(EQ_MUL, 1);

  mul->first_child = *rec;
  mul->next = ((struct eq_node *)*rec)->next;
  
  while(*prev != NULL) {
    
    if((*prev)->type == EQ_RECIPROCAL) {
      eq_move_children(mul, *prev);
      *prev = eq_delete(*prev);
      continue;
    }
    
    prev = (struct eq_node **)&(*prev)->next;
  }
  
ret:
  return;
}