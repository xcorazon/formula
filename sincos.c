#include <stdlib.h>
#include <math.h>

#include "sincos.h"
#include "common.h"
#include "summ.h"


void eq_calculate_sin(struct eq_node **node)
{
  struct eq_leaf *child = (*node)->first_child;
  
  if(child == NULL) {
    struct eq_leaf *zero = eq_leaf_new(EQ_NUMBER, 1, NULL, 0.0);
    zero->next = eq_delete(*node);
    *node = (struct eq_node *)zero;
    return;
  }
  
  if(child->type == EQ_NUMBER) {
    double val = sin(child->sign * child->value * M_PI / 180);
    struct eq_leaf *num = eq_leaf_new(EQ_NUMBER, val, NULL, val);
    num->next = eq_delete(*node);
    *node = (struct eq_node *)num;
    return;
  }
}

void eq_transform_sin(void **node)
{
  struct eq_node *child = ((struct eq_node *)*node)->first_child;
  
  if(child == NULL)
    goto ret;
  
  /* синус нечетная функция,
     поэтому знак выносим за пределы функции */
  ((struct eq_node *)*node)->sign *= child->sign;
  child->sign = 1;
  
  /* под синусом сумма угла и численного значения */
  if(child->type == EQ_SUMM) {
    struct eq_leaf *num = get_summ_number(child);
    
    if(num == NULL)
      goto ret;
    
    double val = num->value;
    double delta = 0;
    val = fmod(val, 360);
    
    if(val >= 180) {
      delta += 180;
      val = fmod(val, 180);
    }
    
    if(val >= 90) {
      delta += 90;
      val = fmod(val, 90);
    }
    
    num->value = val;
    double sin_res = sin(num->sign * delta * M_PI / 180);
    
    if(abs(sin_res) <= 0.00001) {
      ((struct eq_node *)*node)->sign *= round(cos(num->sign * delta * M_PI / 180));
      goto ret;
    }
    
    ((struct eq_node *)*node)->type = EQ_COS;
    ((struct eq_node *)*node)->sign *= round(sin_res);
    
    goto ret;
  }
  
  /* под синусом арксинус */
  if(child->type == EQ_ASIN) {
    struct eq_node *res = child->first_child;
    child->first_child = NULL;
    res->sign *= child->sign * ((struct eq_node *)*node)->sign;
    
    res->next = eq_delete(*node);
    *node = res;
  }
  
ret:
  return;
}

void eq_calculate_cos(struct eq_node **node)
{
  struct eq_leaf *child = (*node)->first_child;
  
  if(child == NULL) {
    struct eq_leaf *one = eq_leaf_new(EQ_NUMBER, 1, NULL, 1);
    one->next = eq_delete(*node);
    *node = (struct eq_node *)one;
    return;
  }
  
  if(child->type == EQ_NUMBER) {
    double val = cos(child->sign * child->value * M_PI / 180);
    struct eq_leaf *num = eq_leaf_new(EQ_NUMBER, val, NULL, val);
    num->next = eq_delete(*node);
    *node = (struct eq_node *)num;
    return;
  }
}


void eq_transform_cos(void **node)
{
  struct eq_node *child = ((struct eq_node *)*node)->first_child;
  
  if(child == NULL)
    goto ret;
  
  /* косинус четная функция,
     поэтому меняем знак выражения под функцией на "+" */
  child->sign = 1;
  
  if(child->type == EQ_SUMM) {
    struct eq_leaf *num = get_summ_number(child);
    
    if(num == NULL)
      goto ret;
    
    double val = num->value;
    double delta = 0;
    val = fmod(val, 360);
    
    if(val >= 180) {
      delta += 180;
      val = fmod(val, 180);
    }
    
    if(val >= 90) {
      delta += 90;
      val = fmod(val, 90);
    }
    
    num->value = val;
    double cos_res = cos(num->sign * delta * M_PI / 180);
    
    if(abs(cos_res) <= 0.00001) {
      ((struct eq_node *)*node)->sign *= -round(sin(num->sign * delta * M_PI / 180));
      ((struct eq_node *)*node)->type = EQ_SIN;
      goto ret;
    }
    
    ((struct eq_node *)*node)->sign *= round(cos_res);
    
    goto ret;
  }
  
  /* под косинусом арккосинус */
  if(child->type == EQ_ACOS) {
    struct eq_node *res = child->first_child;
    child->first_child = NULL;
    res->sign *= child->sign;
    
    res->next = eq_delete(*node);
    *node = res;
  }
  
ret:
  return;
}