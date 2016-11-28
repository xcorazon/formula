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
    if(eq_equals(child, mul_clone, false)) {
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
        child->next = NULL;
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
 * Move multiplier in the brackets
 */
void eq_move_multiplier_in(struct eq_node **node)
{
  if(eq_is_leaf(*node))
    goto ret;
  
  struct eq_node **child = (struct eq_node **)&(*node)->first_child;
  
  while(*child != NULL) {
    eq_move_multiplier_in(child);
    child = (struct eq_node **)&(*child)->next;
  }
  
  if((*node)->type != EQ_MUL)
    goto ret;
  
  // find summ
  struct eq_node *summ = (*node)->first_child;
  struct eq_node **prev = (struct eq_node **)&(*node)->first_child;
  
  while(summ != NULL){
    if(summ->type == EQ_SUMM){
      *prev = (struct eq_node *)summ->next;
      summ->next = (*node)->next;
      (*node)->next = NULL;
      break;
    }
    prev = (struct eq_node **)&summ->next;
    summ = *prev;
  }
  
  if(summ == NULL)
    goto ret;
  
  // move node in summ
  prev = (struct eq_node **)&summ->first_child;
  
  if(*prev != NULL) {
    void *next = (*prev)->next;
    
    while(next != NULL) {
      void *tmp2 = eq_clone(*node);
      *prev = eq_mul((void **)prev, &tmp2);

      (*prev)->next = next;
      prev = (struct eq_node **)&(*prev)->next;
      next = ((struct eq_node *)next)->next;
    }
    *prev = eq_mul((void **)prev, (void **)node);
    (*prev)->next = NULL;
  }
  
  *node = summ;
  
ret:
  return;
}

/*
 * If one of children is mul - his children moves to current node.
 */
void eq_combine_mul(struct eq_node *node)
{
  if(node == NULL || node->type == EQ_SYMBOL || node->type == EQ_NUMBER)
    return;
  
  if(eq_children_count(node) == 0)
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
        continue;
      }
      
      prev = (struct eq_node **)&child->next;
      child = child->next;
    }
  }
}

void eq_calculate_mul(struct eq_node **node)
{
  struct eq_leaf **num = (struct eq_leaf **)&(*node)->first_child;
  struct eq_leaf *leaf;
  struct eq_leaf **prev;
  
  div_symbols(*node);
  
  num = (struct eq_leaf **)&(*node)->first_child;
  
  while(*num != NULL && (*num)->type != EQ_NUMBER)
    num = (struct eq_leaf **)&(*num)->next;
  
  if (*num == NULL)
    goto ret;
  
  prev = (struct eq_leaf **)&(*num)->next;
  leaf = (*num)->next;
  (*num)->value *= (*num)->sign;
  
  while(leaf != NULL) {
    if(leaf->type == EQ_NUMBER) {
      (*num)->value *= leaf->value * leaf->sign;
      *prev = leaf->next;
      eq_delete(leaf);
    } else 
      prev = (struct eq_leaf **)&leaf->next;
    
    leaf = *prev;
  }
  
  (*num)->sign = 1;
  if((*num)->value < 0)
    (*num)->sign = -1;
  (*num)->value = fabs((*num)->value);
  
ret:
  return;
}


void div_symbols(struct eq_node *mul)
{
  struct eq_node **multiplier = (struct eq_node **)&mul->first_child;
  
repeat:
  
  while(*multiplier != NULL) {
    if((*multiplier)->type == EQ_RECIPROCAL) {
      multiplier = (struct eq_node **)&(*multiplier)->next;
      continue;
    }
    
    struct eq_node* child = (*multiplier)->next;
    
    while(child != NULL) {
      if(child->type == EQ_RECIPROCAL) {
        struct eq_node **divisor = (struct eq_node **)&child->first_child;
        
        while((*divisor) != NULL) {
          if(eq_equals(*multiplier, *divisor, false)) {
            mul->sign *= (*divisor)->sign * (*multiplier)->sign;
            *divisor = eq_delete(*divisor);
            *multiplier = eq_delete(*multiplier);
            goto repeat;
          }
          divisor = (struct eq_node **)&(*divisor)->next;
        }
      }
      child = child->next;
    }
    multiplier = (struct eq_node **)&(*multiplier)->next;
  }
}


void eq_transform_mul(void **mul)
{
  struct eq_node *child = ((struct eq_node *)(*mul))->first_child;
  
  if(child == NULL) {
    void *next = eq_delete(*mul);
    *mul = eq_leaf_new(EQ_NUMBER, (*(struct eq_node **)mul)->sign, NULL, 1);
    (*(struct eq_leaf **)mul)->next = next;
    goto ret;
  }
  
  if(eq_children_count(*mul) == 1) {
    void *res = ((struct eq_node *)(*mul))->first_child;
    ((struct eq_node *)(*mul))->first_child = NULL;
    ((struct eq_node *)res)->sign *= ((struct eq_node *)(*mul))->sign;
    ((struct eq_node *)res)->next = eq_delete(*mul);
    *mul = res;
    goto ret;
  } else 
    eq_combine_mul(*mul);
  
  child = ((struct eq_node *)(*mul))->first_child;
  void **prev = &((struct eq_node *)(*mul))->first_child;
  
  while(child != NULL) {
    (*(struct eq_node **)mul)->sign *= child->sign;
    child->sign = 1;
    
    if(child->type == EQ_NUMBER && ((struct eq_leaf *)child)->value == 1)
      *prev = eq_delete(child);
    else
      prev = &child->next;
    
    child =*prev;
  }
  
  // if find zero - delete mul and change it to 0.
  struct eq_leaf *num = (*(struct eq_node **)mul)->first_child;
  
  while(num != NULL && num->type != EQ_NUMBER)
    num = num->next;
  
  if(num != NULL && num->value == 0){
    struct eq_leaf *res = eq_leaf_new(EQ_NUMBER, 1, NULL, 0.0);
    res->next = eq_delete(*mul);
    *mul = res;
  }
  
ret:
  return;
}


void eq_move_multipliers_out(struct eq_node **node)
{
    if(eq_is_leaf(*node))
        goto ret;
    
    struct eq_node *mul = eq_find_common_mul(*node);
    
    if(mul != NULL) {
        eq_move_multiplier_out(node, mul);
        eq_transform_mul(node);
    }
    
    struct eq_node **child = (struct eq_node **)&((*node)->first_child);
    while(*child != NULL) {
        eq_move_multipliers_out(child);
        child = (struct eq_node **)&(*child)->next;
    }
    
ret:
    return;
}