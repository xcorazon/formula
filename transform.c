#include <stdlib.h>
#include "transform.h"
#include "eqtypes.h"
#include "common.h"
#include "summ.h"
#include "mul.h"
#include "reciprocal.h"
#include "sincos.h"

void eq_nothing(struct eq_node **node) {return;}
void transform_nothing(void **node) {return;}

static void (* calculate[])(struct eq_node **node) = {
              NULL,
              eq_nothing,
              eq_nothing,
              eq_calculate_summ,
              eq_calculate_mul,
              eq_calculate_reciprocal,
              eq_calculate_sin
              };
               
static void (* transform[])(void **node) = {
                NULL,
                transform_nothing,
                transform_nothing,
                eq_transform_summ,
                eq_transform_mul,
                eq_transform_reciprocal,
                eq_transform_sin
                };
                

void eq_calculate(void **node)
{
  if(eq_is_leaf(*node))
    goto ret;
  
  if(!eq_is_leaf(*node))
    eq_transform_children((struct eq_node *)*node, eq_calculate);
  
  calculate[((struct eq_node *)*node)->type]((struct eq_node **)node);
  
ret:
  return;
}


void eq_transform(void **node)
{
  if(eq_is_leaf(*node))
    goto ret;
  
  if(!eq_is_leaf(*node))
    eq_transform_children((struct eq_node *)*node, eq_transform);
  
  transform[((struct eq_node *)*node)->type](node);
  
ret:
  return;
}

void eq_transform_children(struct eq_node *node, void (*transform)(void **))
{
  void **prev;
  struct eq_node *child;
  if(transform != NULL) {
    child = node->first_child;
    prev = &node->first_child;
    while(child != NULL) {
      transform(prev);
      prev = &(*(struct eq_node **)prev)->next;
      child = *prev;
    }
  }
}