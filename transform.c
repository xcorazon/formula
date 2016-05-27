#include <stdlib.h>
#include "transform.h"
#include "eqtypes.h"
#include "common.h"
#include "summ.h"
#include "mul.h"

void eq_nothing(struct eq_node *node, void (*calc)(void *)) {return;}
void transform_nothing(void **node, void (*transform)(void **)) {return;}

static void (* calculate[])(struct eq_node *node, void (*)(void *)) = {
              NULL,
              eq_nothing,
              eq_nothing,
              eq_calculate_summ,
              eq_calculate_mul
              };
               
static void (* transform[])(void **node, void (*)(void **)) = {
                NULL,
                transform_nothing,
                transform_nothing,
                eq_transform_summ,
                eq_transform_mul
                };
                

void eq_calculate(void *node)
{
  if(eq_is_leaf(node))
    goto ret;
  
  calculate[((struct eq_node *)node)->type]((struct eq_node *)node, eq_calculate);
  
ret:
  return;
}

void eq_transform(void **node)
{
  if(eq_is_leaf(*node))
    goto ret;
  
  transform[((struct eq_node *)*node)->type](node, eq_transform);
  if(!eq_is_leaf(*node))
    eq_transform_children((struct eq_node *)*node, eq_transform);
  
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