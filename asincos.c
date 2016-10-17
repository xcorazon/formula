#include <stdlib.h>
#include <math.h>

#include "asincos.h"
#include "common.h"
#include "summ.h"


void eq_calculate_asin(struct eq_node **node)
{
  struct eq_leaf *child = (*node)->first_child;
  
  if(child == NULL) {
    struct eq_leaf *zero = eq_leaf_new(EQ_NUMBER, 1, NULL, 0.0);
    zero->next = eq_delete(*node);
    *node = (struct eq_node *)zero;
    return;
  }
  
  if(eq_is_leaf(child)) {
    /* TODO: Сделать проверку ошибки */
    if(child->type == EQ_NUMBER) {
      child->value = (*node)->sign * asin(child->value * child->sign);
      child->sign = 1;
      
      if(child->value < 0) {
        child->sign = -1;
        child->value = -child->value;
      }
      
      (*node)->first_child = NULL;
      child->next = eq_delete(*node);
      *node = (struct eq_node *)child;
    }
  }
}

void eq_transform_asin(void **node)
{
  struct eq_node *child = ((struct eq_node *)*node)->first_child;
  
  if(child == NULL) goto ret;
  
  if(child->type == EQ_SIN) {
    struct eq_node *angle = child->first_child;
    angle->sign *= child->sign;
    angle->next = eq_delete(*node);
    *node = angle;
  }

ret:
  return;
}

void eq_calculate_acos(struct eq_node **node)
{
  struct eq_leaf *child = (*node)->first_child;
  
  if(child == NULL) {
    struct eq_leaf *num = eq_leaf_new(EQ_NUMBER, 1, NULL, 90.0);
    num->next = eq_delete(*node);
    *node = (struct eq_node *)num;
    return;
  }
  
  if(eq_is_leaf(child)) {
    /* TODO: Сделать проверку ошибки */
    if(child->type == EQ_NUMBER) {
      child->value = (*node)->sign * acos(child->value * child->sign);
      child->sign = 1;
      
      if(child->value < 0) {
        child->sign = -1;
        child->value = -child->value;
      }
      
      (*node)->first_child = NULL;
      child->next = eq_delete(*node);
      *node = (struct eq_node *)child;
    }
  }
}


void eq_transform_acos(void **node)
{
  struct eq_node *child = ((struct eq_node *)*node)->first_child;
  
  if(child == NULL) goto ret;
  
  if(child->type == EQ_COS) {
    
    if(child->sign > 0) {
      
      struct eq_node *angle = child->first_child;
      child->first_child = NULL;
      
      angle->next = eq_delete(*node);
      
      *node = angle;
      
    } else {
      /* arcos(-cos(a)) = (a + 180) */
      
      struct eq_node *angle = child->first_child;
      struct eq_node *summ = eq_node_new(EQ_SUMM, 1);
      struct eq_leaf *num = eq_leaf_new(EQ_NUMBER, 1, "", 180.0);
      
      child->first_child = NULL;
      summ->first_child = angle;
      
      angle->next = num;
      
      summ->next = eq_delete(*node);
      *node = summ;
    }
      
  }

ret:
  return;
}