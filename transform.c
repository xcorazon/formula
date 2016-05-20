#include <stdlib.h>
#include "transform.h"
#include "eqtypes.h"
#include "common.h"
#include "summ.h"
#include "mul.h"

void eq_nothing(struct eq_node *node, void (*calc)(void *)) {return;}

static void (* calculate[])(struct eq_node *node, void (*)(void *)) = {
              NULL,
              eq_nothing,
              eq_nothing,
              eq_calculate_summ,
              eq_calculate_mul
              };
               
static void * (* transform[])(void **node, void *(*)(void **)) = {
                NULL/*,
                eq_transform_symbol,
                eq_transform_number,
                eq_transform_summ,
                eq_transform_mul*/
                };
                

void eq_calculate(void *node)
{
  if(eq_is_leaf(node))
    goto ret;
  
  calculate[(struct eq_node *)node->type]((struct eq_node *)node, eq_calculate);
  
ret:
  return;
}

void *eq_transform(void **node)
{
  return NULL;
}