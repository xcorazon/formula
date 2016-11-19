#include "common.h"
#include "treeview.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define MAX_CHARS 150

void tv_leaf(void *node, int tab, wchar_t *result);
void tv_node(void *node, int tab, wchar_t *result);

static void (* to_string[])(void *node, int flags, wchar_t *result) = {
      NULL,
      tv_leaf, 
      tv_leaf,
      tv_node,
      tv_node,
      tv_node,
      tv_node,
      tv_node,
      tv_node,
      tv_node
};
      
static wchar_t *types[] = {
      L"",
      L"Symbol",
      L"Number",
      L"Summ",
      L"Mul",
      L"Reciprocal",
      L"Sin",
      L"Cos",
      L"Asin",
      L"Acos",
};


void tree_view(struct eq_node *node, int tab, wchar_t *result)
{
  if(eq_is_leaf(node))
    tv_leaf((void *)node, tab, result);
  else
    tv_node((void *)node, tab, result);
}


void tv_leaf(void *node, int tab, wchar_t *result)
{
  wchar_t tb[100];
  wchar_t *nl = L"\n";
  wchar_t tmp[MAX_CHARS];
  
  wmemset(tb, 0, 100);
  wmemset(tb, L' ', tab);
  wchar_t sign = L'+';
  if(((struct eq_leaf *)node)->sign < 0)
    sign='-';
  
  if(((struct eq_leaf *)node)->type == EQ_SYMBOL)
    swprintf(tmp, MAX_CHARS-1, L"type: %ls  %lc%ls", types[((struct eq_leaf *)node)->type], sign, ((struct eq_leaf *)node)->name);
  else
    swprintf(tmp, MAX_CHARS-1, L"type: %ls  %lc%f", types[((struct eq_leaf *)node)->type], sign, ((struct eq_leaf *)node)->value);
  
  wcscat(result, nl);
  wcscat(result, tb);
  wcscat(result, tmp);
}


void tv_node(void *node, int tab, wchar_t *result)
{
  wchar_t tb[100];
  wchar_t *nl = L"\n";
  wchar_t tmp[MAX_CHARS];
  
  wmemset(tb, 0, 100);
  wmemset(tb, L' ', tab);
  
  wchar_t sign = L'+';
  if(((struct eq_node *)node)->sign < 0)
    sign=L'-';
  
  swprintf(tmp, MAX_CHARS-1, L"type: %ls sign:\'%lc\'", types[((struct eq_node *)node)->type], sign);
  
  wcscat(result, nl);
  wcscat(result, tb);
  wcscat(result, tmp);
  
  struct eq_node *child = ((struct eq_node *)node)->first_child;
  while(child != NULL) {
    to_string[child->type](child, tab+4, result);
    child = child->next;
  }
}
