#include "eqtypes.h"
#include "starmath.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>


static void (* to_string[])(struct eq_node *node, int flags, char *result) = {
      NULL,
      sm_symbol, 
      sm_number,
      sm_summ
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
  
  int ts_flags = SM_DEFAULT;
  struct eq_node *child = (struct eq_node *)node->first_child;
  while(child != NULL) {
    to_string[child->type]((struct eq_node *)child, ts_flags, tmp);
    strcat(result, tmp);
    ts_flags = SM_SHOW_SIGN;
    child = (struct eq_node *)child->next;
  }
  
  strcat(result, close_bracket);
}

void sm_to_string(struct eq_node *node, int flags, char *result)
{
  
}