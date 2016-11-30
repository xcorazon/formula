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
  struct eq_leaf *a1 = eq_leaf_new(EQ_SYMBOL, 1, L"a_1", 0);
  struct eq_leaf *c2 = eq_leaf_new(EQ_SYMBOL, 1, L"c_2", 20.345);
  struct eq_leaf *num = eq_leaf_new(EQ_NUMBER, -1, NULL, 5.1);
  
  struct eq_node *summ = eq_node_new(EQ_SUMM, 1);
  
  a1->next = c2;
  c2->next = num;
  summ->first_child = a1;
  
  struct eq_leaf *b = eq_leaf_new(EQ_SYMBOL, 1, L"b", 0);
  struct eq_node *mul  = eq_node_new(EQ_MUL, 1);
  struct eq_node *rec  = eq_node_new(EQ_RECIPROCAL, 1);
  rec->first_child = summ;
  mul->first_child = b;
  b->next = rec;
  
  eq_transform(&mul);
  
  struct eq_node *mul2  = eq_node_new(EQ_MUL, 1);
  struct eq_node *rec2  = eq_node_new(EQ_RECIPROCAL, 1);
  mul2->first_child = mul;
  mul->next = rec2;
  
  rec2->first_child = eq_leaf_new(EQ_SYMBOL, 1, L"b_2", 0);
  
  *treeview_result = 0;
  tree_view(mul2, 4, treeview_result);
  debug(L"treeview 1 \n %s", treeview_result);
  
  eq_transform(&mul2);
  //eq_transform(&mul2);
  
  *treeview_result = 0;
  tree_view(mul2, 4, treeview_result);
  debug(L"treeview 2 \n%s", treeview_result);
  
  *starmath_result = 0;
  sm_to_string(mul2, SM_DEFAULT, starmath_result);
  debug(L"%s",starmath_result);
  
  *treeview_result = 0;
  tree_view(mul2, 4, treeview_result);
  debug(L"%s", treeview_result);
  
  eq_delete(mul2);
    
  return 0;
}
