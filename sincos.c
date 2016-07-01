#include <stdlib.h>

#include "sincos.h"
#include "common.h"
#include "summ.h"
#include "math.h"


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
  
  /* ����� �������� �������,
     ������� ���� ������� �� ������� ������� */
  ((struct eq_node *)*node)->sign *= child->sign;
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
    double sin_res = sin(num->sign * val * M_PI / 180);
    
    if(sin_res == 0) {
      ((struct eq_node *)*node)->sign *= cos(num->sign * val * M_PI / 180);
      goto ret;
    }
    
    ((struct eq_node *)*node)->type = EQ_COS;
    ((struct eq_node *)*node)->sign *= sin_res;
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
  
  /* ������� ������ �������,
     ������� ������ ���� ��������� ��� �������� �� "+" */
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
    double cos_res = cos(num->sign * val * M_PI / 180);
    
    if(cos_res == 0) {
      ((struct eq_node *)*node)->sign *= -sin(num->sign * val * M_PI / 180);
      ((struct eq_node *)*node)->type = EQ_SIN;
      goto ret;
    }
    
    ((struct eq_node *)*node)->sign *= cos_res;
  }
  
ret:
  return;
}