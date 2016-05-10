#include "eqtypes.h"
#include "common.h"
#include "summ.h"

struct eq_node *eq_add(void *node1, void *node2)
{
  struct eq_node *summ;
  
  if((struct eq_node *)node1->type == EQ_SUMM) {
    summ = (struct eq_node *)node1;
    eq_move_children(summ, (struct eq_node *)node2);
    eq_delete(node2);
  }
  else if((struct eq_node *)node2->type == EQ_SUMM) {
    summ = (struct eq_node *)node2;
    eq_move_children(summ, (struct eq_node *)node1);
    eq_delete(node1);
  }
  else {
    summ = eq_node_new(EQ_SUMM, 1);
    summ->first_child = node1;
    ((struct eq_node *)node1)->next = node2;
  }
  
  return summ;
}