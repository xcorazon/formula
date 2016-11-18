#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <math.h>
#include "eqtypes.h"
#include "starmath.h"


static void (* to_string[])(struct eq_node *node, int flags, wchar_t *result) = {
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

void sm_symbol(struct eq_node *node, int flags, wchar_t *result)
{
  wchar_t *sign = L"";
  if((flags & SM_SHOW_SIGN) != 0)
    sign = L"+";
  if(node->sign < 0)
    sign = L"-";

  wcscpy(result, sign);
  wcscat(result, ((struct eq_leaf *)node)->name);
}

void sm_number(struct eq_node *node, int flags, wchar_t *result)
{
  wchar_t *sign = L"";
  wchar_t tmp[20];
  *tmp = 0;
  
  if((flags & SM_SHOW_SIGN) != 0)
    sign = L"+";
  if(node->sign < 0)
    sign = L"-";
  
  wcscpy(result, sign);
  swprintf(tmp, 19, L"%.2f", ((struct eq_leaf *)node)->value);
  wchar_t *pos = wcsstr(tmp, L".00");
  if(pos != NULL)
    *pos = L'\0';
  wcscat(result, tmp);
}

void sm_summ(struct eq_node *node, int flags, wchar_t *result)
{
  wchar_t tmp[1000];
  wchar_t *open_bracket = L"{";
  wchar_t *close_bracket = L"}";
  *tmp = 0;
  wchar_t *sign = L"";
  if((flags & SM_SHOW_SIGN) != 0)
    sign = L"+";
  if(node->sign < 0)
    sign = L"-";
  if(node->sign < 0 || (flags & SM_ROUND_BRACKET) != 0) {
    open_bracket  = L"(";
    close_bracket = L")";
  }
  wcscpy(result, sign);
  wcscat(result, open_bracket);
  
  int ts_flags = SM_DEFAULT & FR_SUMM;
  struct eq_node *child = (struct eq_node *)node->first_child;
  while(child != NULL) {
    to_string[child->type]((struct eq_node *)child, ts_flags, tmp);
    wcscat(result, tmp);
    ts_flags = SM_SHOW_SIGN;
    child = (struct eq_node *)child->next;
  }
  
  wcscat(result, close_bracket);
}

void sm_mul(struct eq_node *node, int flags, wchar_t *result)
{
  wchar_t tmp[1000];
  wchar_t *open_bracket = L"{";
  wchar_t *close_bracket = L"}";
  *tmp = 0;
  struct eq_node *child = node->first_child;
  
  wchar_t s = node->sign;
  wchar_t *sign = L"";
  while(child != NULL) {
    s *= child->sign;
    child = child->next;
  }
  if(s < 0)
    sign = L"-";
  else if ((flags & SM_SHOW_SIGN) != 0)
    sign = L"+";
  
  wcscpy(result, sign);
  wcscat(result, open_bracket);
  
  int ts_flags = SM_ROUND_BRACKET & FR_MUL;
  child = (struct eq_node *)node->first_child;
  while(child != NULL) {
    to_string[child->type]((struct eq_node *)child, ts_flags, tmp);
    wcscat(result, tmp);
    child = child->next;
    if(child != NULL)
      wcscat(result, L" cdot ");
  }
  wcscat(result, close_bracket);
}

void sm_reciprocal(struct eq_node *node, int flags, wchar_t *result)
{
  wchar_t *open_bracket = L"{";
  wchar_t *close_bracket = L"}";
  wchar_t tmp[1000];
  *tmp = 0;
  
  if((flags & (0xFFFF << 16)) != FR_MUL) {
    wcscpy(tmp, open_bracket);
    wcscat(tmp, L"{1}");
  }
  
  if(node->type == EQ_RECIPROCAL) {
    wcscat(tmp, L" over ");
    node->type = EQ_MUL;
    char tmp_sign = node->sign;
    node->sign = 1;
    
    sm_mul(node, SM_DEFAULT, tmp);
    
    node->type = EQ_RECIPROCAL;
    node->sign = tmp_sign;
    
    if((flags & (0xFFFF << 16)) != FR_MUL)
      wcscat(tmp, close_bracket);
    
    wcscat(result, tmp);
  }
}

void sm_sin(struct eq_node *node, int flags, wchar_t *result)
{
  
}

void sm_cos(struct eq_node *node, int flags, wchar_t *result)
{
  
}

void sm_asin(struct eq_node *node, int flags, wchar_t *result)
{
  
}


void sm_acos(struct eq_node *node, int flags, wchar_t *result)
{
  
}


void sm_to_string(struct eq_node *node, int flags, wchar_t *result)
{
  
}