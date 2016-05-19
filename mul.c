#include <stdlib.h>
#include <stdbool.h>
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