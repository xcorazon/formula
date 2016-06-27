#include "treeview.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void tv_leaf(void *node, int tab, char *result);
void tv_node(void *node, int tab, char *result);

static void (* to_string[])(void *node, int flags, char *result) = {
      NULL,
      tv_leaf, 
      tv_leaf,
      tv_node,
      tv_node,
      tv_node
};
      
static char *types[] = {
      "",
      "Symbol",
      "Number",
      "Summ",
      "Mul",
      "Reciprocal"
};


void tree_view(struct eq_node *node, int tab, char *result)
{
  if(eq_is_leaf(node))
    tv_leaf((void *)node, tab, result);
  else
    tv_node((void *)node, tab, result);
}


void tv_leaf(void *node, int tab, char *result)
{
  char tb[100];
  char *nl = "\n";
  char tmp[150];
  
  memset(tb, 0, 100);
  memset(tb, ' ', tab);
  char sign = '+';
  if(((struct eq_leaf *)node)->sign < 0)
    sign='-';
  
  if(((struct eq_leaf *)node)->type == EQ_SYMBOL)
    sprintf(tmp, "type: %s  %c%s", types[((struct eq_leaf *)node)->type], sign, ((struct eq_leaf *)node)->name);
  else
    sprintf(tmp, "type: %s  %c%f", types[((struct eq_leaf *)node)->type], sign, ((struct eq_leaf *)node)->value);
  
  strcat(result, nl);
  strcat(result, tb);
  strcat(result, tmp);
}


void tv_node(void *node, int tab, char *result)
{
  char tb[100];
  char *nl = "\n";
  char tmp[150];
  
  memset(tb, 0, 100);
  memset(tb, ' ', tab);
  
  char sign = '+';
  if(((struct eq_node *)node)->sign < 0)
    sign='-';
  
  sprintf(tmp, "type: %s sign:\'%c\'", types[((struct eq_node *)node)->type], sign);
  
  strcat(result, nl);
  strcat(result, tb);
  strcat(result, tmp);
  
  struct eq_node *child = ((struct eq_node *)node)->first_child;
  while(child != NULL) {
    to_string[child->type](child, tab+4, result);
    child = child->next;
  }
}