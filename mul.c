#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "mul.h"
#include "summ.h"
#include "eqtypes.h"
#include "common.h"

/*
 * Mul two equations
 */
struct eq_node *eq_mul(void **node1, void **node2)
{
  struct eq_node *mul;
  
  mul = eq_node_new(EQ_MUL, 1);
  mul->first_child = *node1;
  ((struct eq_node *)(*node1))->next = *node2;
  eq_combine_mul(mul);

  return mul;
}

/*
 * Move multiplier out the brackets
 */
void eq_move_multiplier_out(struct eq_node **node, struct eq_node *multiplier)
{
  if((*node)->type != EQ_SUMM)
    goto ret;
  
  void *summ = eq_node_new(EQ_SUMM, 1);
  struct eq_node *mul = eq_node_new(EQ_MUL, 1);
  struct eq_node *mul_clone = eq_clone(multiplier);
  
  mul->first_child = mul_clone;
  
  struct eq_node *child = (*node)->first_child;
  struct eq_node **prev = (struct eq_node **)&(*node)->first_child;
  
  while(child != NULL) {
    if(eq_is_leaf(child) && eq_equals(child, mul_clone, false)) {
      *prev = child->next;
      void *tmp = eq_leaf_new(EQ_NUMBER, child->sign * mul_clone->sign, NULL, 1.0);
      summ = eq_add(&summ, &tmp);
      eq_delete(child);
      child = *prev;
      continue;
    }
    
    if(child->type == EQ_MUL) {
      struct eq_node *tmp = eq_find(child, mul_clone);
      if(tmp != NULL) {
        eq_remove_child(child, mul_clone);
        *prev = child->next;
        summ = eq_add(&summ, (void **)&child);
        child = *prev;
        continue;
      }
    }
    
    prev = (struct eq_node **)&child->next;
    child = child->next;
  }
  mul_clone->next = summ;
  *node = eq_add((void **)node, (void **)&mul);
  
ret:
  return;
}

/*
 * If one of children is summ - his children moves to current node.
 */
void eq_combine_mul(struct eq_node *node)
{
  if(node == NULL || node->type == EQ_SYMBOL || node->type == EQ_NUMBER)
    return;
  
  struct eq_node *child = node->first_child;
  struct eq_node **prev = (struct eq_node **)(&node->first_child);
  
  while(child != NULL) {
    eq_combine_mul(child);
    child = child->next;
  }
  
  child = node->first_child;
  
  if(node->type == EQ_MUL) {
    while(child != NULL) {
      
      if(child->type == EQ_MUL) {
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

void eq_calculate_mul(struct eq_node *node, void (*calculate)(void *))
{
  struct eq_leaf *num = node->first_child;
  struct eq_leaf *leaf;
  struct eq_leaf **prev;
  
  if(calculate != NULL) {
    while(num != NULL) {
      calculate(num);
      num = num->next;
    }
  }
  
  num = node->first_child;
  
  while(num != NULL && num->type != EQ_NUMBER)
    num = num->next;
  
  if (num == NULL)
    goto ret;
  
  prev = (struct eq_leaf **)&num->next;
  leaf = num->next;
  num->value *= num->sign;
  while(leaf != NULL) {
    if(leaf->type == EQ_NUMBER) {
      num->value *= leaf->value * leaf->sign;
      *prev = leaf->next;
      eq_delete(leaf);
    } else 
      prev = (struct eq_leaf **)&leaf->next;
    
    leaf = *prev;
  }
  num->sign = 1;
  if(num->value < 0)
    num->sign = -1;
  num->value = fabs(num->value);
ret:
  return;
}