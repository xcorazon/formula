#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "eqtypes.h"
#include "starmath.h"


static void (* to_string[])(struct eq_node *node, int flags, char *result) = {
      NULL,
      sm_symbol, 
      sm_number,
      sm_summ,
      sm_mul,
      sm_reciprocal,
      sm_sin,
      sm_cos,
      sm_asin,
      sm_acos
      };

void sm_symbol(struct eq_node *node, int flags, char *result)
{
  char *sign = "";
  if((flags & SM_SHOW_SIGN) != 0)
    sign = "+";
  if(node->sign < 0)
    sign = "-";

  strcpy(result, sign);
  strcat(result, ((struct eq_leaf *)node)->name);
}

void sm_number(struct eq_node *node, int flags, char *result)
{
  char *sign = "";
  char tmp[20];
  *tmp = 0;
  
  if((flags & SM_SHOW_SIGN) != 0)
    sign = "+";
  if(node->sign < 0)
    sign = "-";
  
  strcpy(result, sign);
  snprintf(tmp, 19, "%.2f", ((struct eq_leaf *)node)->value);
  char *pos = strstr(tmp, ".00");
  if(pos != NULL)
    *pos = '\0';
  strcat(result, tmp);
}

void sm_summ(struct eq_node *node, int flags, char *result)
{
  char tmp[1000];
  char *open_bracket = "{";
  char *close_bracket = "}";
  *tmp = 0;
  char *sign = "";
  if((flags & SM_SHOW_SIGN) != 0)
    sign = "+";
  if(node->sign < 0)
    sign = "-";
  if(node->sign < 0 || (flags & SM_ROUND_BRACKET) != 0) {
    open_bracket  = "(";
    close_bracket = ")";
  }
  strcpy(result, sign);
  strcat(result, open_bracket);
  
  int ts_flags = SM_DEFAULT & FR_SUMM;
  struct eq_node *child = (struct eq_node *)node->first_child;
  while(child != NULL) {
    to_string[child->type]((struct eq_node *)child, ts_flags, tmp);
    strcat(result, tmp);
    ts_flags = SM_SHOW_SIGN;
    child = (struct eq_node *)child->next;
  }
  
  strcat(result, close_bracket);
}

void sm_mul(struct eq_node *node, int flags, char *result)
{
  char tmp[1000];
  char *open_bracket = "{";
  char *close_bracket = "}";
  *tmp = 0;
  struct eq_node *child = node->first_child;
  
  char s = node->sign;
  char *sign = "";
  while(child != NULL) {
    s *= child->sign;
    child = child->next;
  }
  if(s < 0)
    sign = "-";
  else if ((flags & SM_SHOW_SIGN) != 0)
    sign = "+";
  
  strcpy(result, sign);
  strcat(result, open_bracket);
  
  int ts_flags = SM_ROUND_BRACKET & FR_MUL;
  child = (struct eq_node *)node->first_child;
  while(child != NULL) {
    to_string[child->type]((struct eq_node *)child, ts_flags, tmp);
    strcat(result, tmp);
    child = child->next;
    if(child != NULL)
      strcat(result, " cdot ");
  }
  strcat(result, close_bracket);
}

void sm_reciprocal(struct eq_node *node, int flags, char *result)
{
  char *open_bracket = "{";
  char *close_bracket = "}";
  char tmp[1000];
  *tmp = 0;
  
  if((flags & (0xFFFF << 16)) != FR_MUL) {
    strcpy(tmp, open_bracket);
    strcat(tmp, "{1}");
  }
  
  if(node->type == EQ_RECIPROCAL) {
    strcat(tmp, " over ");
    node->type = EQ_MUL;
    char tmp_sign = node->sign;
    node->sign = 1;
    
    sm_mul(node, SM_DEFAULT, tmp);
    
    node->type = EQ_RECIPROCAL;
    node->sign = tmp_sign;
    
    if((flags & (0xFFFF << 16)) != FR_MUL)
      strcat(tmp, close_bracket);
    
    strcat(result, tmp);
  }
}

void sm_sin(struct eq_node *node, int flags, char *result)
{
  
}

void sm_cos(struct eq_node *node, int flags, char *result)
{
  
}

void sm_asin(struct eq_node *node, int flags, char *result)
{
  
}


void sm_acos(struct eq_node *node, int flags, char *result)
{
  
}


void sm_to_string(struct eq_node *node, int flags, char *result)
{
  
}