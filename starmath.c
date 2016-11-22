#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <math.h>
#include "eqtypes.h"
#include "starmath.h"
#include "common.h"

#ifdef OS_WINDOWS
  #define swprintf _snwprintf
#endif /* OS_WINDOWS */


static void (* to_string[])(struct eq_node *node, int flags, wchar_t *result) = {
      NULL,
      sm_symbol, 
      sm_number,
      sm_summ,
      sm_mul,
      sm_mul,
      sm_sincos,
      sm_sincos,
      sm_asincos,
      sm_asincos
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
    open_bracket  = L" left (";
    close_bracket = L" right )";
  }
  wcscat(result, sign);
  wcscat(result, open_bracket);
  
  int ts_flags = SM_DEFAULT & FR_SUMM;
  struct eq_node *child = (struct eq_node *)node->first_child;
  while(child != NULL) {
    tmp[0] = 0;
    to_string[child->type]((struct eq_node *)child, ts_flags, tmp);
    wcscat(result, tmp);
    ts_flags = SM_SHOW_SIGN;
    child = (struct eq_node *)child->next;
  }
  
  wcscat(result, close_bracket);
}

void sm_mul(struct eq_node *node, int flags, wchar_t *result)
{
    wchar_t numerator[1000];
    wchar_t denom[1000];

    wchar_t *open_bracket = L"{";
    wchar_t *close_bracket = L"}";

    static wchar_t *dot = L" cdot ";

    *numerator = 0;
    *denom     = 0;
    struct eq_node *child = node->first_child;

    char s = node->sign;
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

    int ts_flags = SM_ROUND_BRACKET | FR_MUL;
    child = (struct eq_node *)node->first_child;

    wchar_t *ndot = L"";
    wchar_t *ddot = L"";

    while(child != NULL) {
        if (child->type == EQ_RECIPROCAL) {
            wcscat(denom, ddot);
            sm_mul((struct eq_node *)child, FR_MUL, denom);
            ddot = dot;
        } else {
            wcscat(numerator, ndot);
            to_string[child->type]((struct eq_node *)child, ts_flags, numerator);
            ndot = dot;
        }
        child = child->next;
    }

    mul_to_str(flags, node->type, numerator, denom, result);

    wcscat(result, close_bracket);
}

void mul_to_str(int flags, char type, wchar_t *numerator, wchar_t *denom, wchar_t *result)
{
    if (!(flags & FR_MUL && type == EQ_MUL)) {
        wchar_t *tmp = denom;
        denom     = numerator;
        numerator = tmp;
    }
    
    if (*denom == 0) {
        wcscat(result, numerator);
    } else if (*numerator == 0) {
        wcscat(result, L"1 over {");
        wcscat(result, denom);
        wcscat(result, L"}");
    } else {
        wcscat(result, L"{");
        wcscat(result, numerator);
        wcscat(result, L"} over {");
        wcscat(result, denom);
        wcscat(result, L"}");
    }
}


void sm_sincos(struct eq_node *node, int flags, wchar_t *result)
{
    wchar_t *open_bracket = L"left (";
    wchar_t *close_bracket = L"right )";
    
    struct eq_node *child = (struct eq_node *)node->first_child;
    
    if ((child->type == EQ_SYMBOL || child->type == EQ_NUMBER) && child->sign > 0) {
        open_bracket = L"{";
        close_bracket = L"}";
    }
    
    char s = node->sign;
    wchar_t *sign = L"";
    
    if(s < 0)
        sign = L"-";
    else if ((flags & SM_SHOW_SIGN) != 0)
        sign = L"+";
    
    wcscat(result, sign);
    if(node->type == EQ_SIN)
        wcscat(result, L"sin ");
    else
        wcscat(result, L"cos ");
    wcscat(result, open_bracket);
    
    
    
    while(child != NULL) {
        to_string[child->type](child, SM_DEFAULT, result);
        child = child->next;
    }
    
    wcscat(result, close_bracket);
}


void sm_asincos(struct eq_node *node, int flags, wchar_t *result)
{
    wchar_t *open_bracket = L"left (";
    wchar_t *close_bracket = L"right )";

    struct eq_node *child = (struct eq_node *)node->first_child;
    
    if ((child->type == EQ_SYMBOL || child->type == EQ_NUMBER) && child->sign > 0) {
        open_bracket = L"{";
        close_bracket = L"}";
    }
    
    char s = node->sign;
    wchar_t *sign = L"";
    
    if(s < 0)
        sign = L"-";
    else if ((flags & SM_SHOW_SIGN) != 0)
        sign = L"+";
    
    wcscat(result, sign);
    if(node->type == EQ_ASIN)
        wcscat(result, L"arcsin ");
    else
        wcscat(result, L"arccos ");
    wcscat(result, open_bracket);
    
    while(child != NULL) {
        to_string[child->type](child, SM_DEFAULT, result);
        child = child->next;
    }
    
    wcscat(result, close_bracket);
}


void sm_to_string(struct eq_node *node, int flags, wchar_t *result)
{
  
}
