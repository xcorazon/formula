#include <stdio.h>
#include <math.h>
#include <wchar.h>
#include <locale.h>

#include "common.h"
#include "starmath.h"
#include "treeview.h"
#include "summ.h"
#include "mul.h"
#include "transform.h"
#include "debug/debug.h"

int main(int argc, char *argv[])
{
  wchar_t starmath_result[1000];
  wchar_t treeview_result[1000];
  
  //struct eq_node *head = eq_node_new(EQ_SUMM, 1);
  struct eq_leaf *a1 = eq_leaf_new(EQ_SYMBOL, 1, L"a", 0);
  struct eq_leaf *a2 = eq_leaf_new(EQ_SYMBOL, 1, L"a", 20.345);
  struct eq_leaf *num = eq_leaf_new(EQ_NUMBER, 1, NULL, 2);
  
  struct eq_leaf *b = eq_leaf_new(EQ_SYMBOL, 1, L"b", 0);
  struct eq_node *mul  = eq_node_new(EQ_MUL, 1);
  struct eq_node *rec  = eq_node_new(EQ_RECIPROCAL, 1);
  
  mul->first_child = a1;
  a1->next = rec;
  rec->next = b;
  
  rec->first_child = a2;
  a2->next = num;
  
  eq_transform(&mul);
  eq_calculate(&mul);
  
  *treeview_result = 0;
  tree_view(mul, 4, treeview_result);
  debug(L"treeview 1 \n %s", treeview_result);
  
  *starmath_result = 0;
  sm_to_string(mul, SM_DEFAULT, starmath_result);
  debug(L"%s",starmath_result);
  
  eq_transform(&mul);
  
  eq_delete(mul);
    
  return 0;
}
