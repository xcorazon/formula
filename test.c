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
  //struct eq_node *head = eq_node_new(EQ_SUMM, 1);
  struct eq_node *summ = eq_node_new(EQ_SUMM, 1);
  struct eq_node *mul  = eq_node_new(EQ_MUL, 1);
  struct eq_node *rec  = eq_node_new(EQ_RECIPROCAL, 1);
  //struct eq_node *sinus  = eq_node_new(EQ_SIN, 1);
  //struct eq_node *asinus  = eq_node_new(EQ_ASIN, -1);
  struct eq_leaf *a = eq_leaf_new(EQ_SYMBOL, 1, L"a", 0);
  struct eq_leaf *b = eq_leaf_new(EQ_NUMBER, 1, L"b", 180);
  struct eq_leaf *c = eq_leaf_new(EQ_SYMBOL, -1, L"a", 0);
  struct eq_leaf *b1 = eq_leaf_new(EQ_SYMBOL, 1, L"b_1", 20.345);

  setlocale(LC_ALL, "en_US.utf8");
  
  wprintf(L"cos(0.001) = %f, round = %i \n", cos(0.001), (int)round(cos(0.001)));
  
  wprintf(L"Size of wchar_t = %d\n", sizeof(wchar_t));
  
  rec->first_child = b1;
  c->next = rec;
  mul->first_child = c;
  
  summ->first_child = a;
  a->next = mul;
  mul->next = b;
    
  wchar_t starmath_result[1000];
  wchar_t treeview_result[1000];
  
  *starmath_result = 0;
  sm_summ(summ, SM_ROUND_BRACKET, starmath_result);
  wprintf(starmath_result);
  
  *treeview_result = 0;
  tree_view(summ, SM_ROUND_BRACKET, treeview_result);
  wprintf(treeview_result);
  
  eq_calculate((void**)&summ);
  
  *treeview_result = 0;
  tree_view(summ, 0, treeview_result);
  debug(treeview_result);
  
  wprintf(L"\ntesting %f\n", 20.1);
  
  wprintf(L"\nresult: %s\n", treeview_result);
  
  eq_transform((void**)&summ);
  wprintf(L"\nhead transformed...\n");  
  
  eq_combine_summ(summ);
  eq_calculate_summ(&summ);
  
  *starmath_result = 0;
  sm_summ(summ, 0, starmath_result);
  debug(starmath_result);

  struct eq_node *multiplier = eq_find_common_mul(summ);
  wprintf(L"\nfinded common multiplier...\n");
  
  eq_move_multipliers_out(&summ);
  wprintf(L"\nmultiplier moved out...\n");

  *treeview_result = 0;
  tree_view(summ, 0, treeview_result);
  wprintf(L"\nAfter move multiplier\n%s", treeview_result);
  
  eq_calculate((void**)&summ);
  eq_calculate((void**)&summ);
  
  tree_view(summ, 0, treeview_result);
  wprintf(treeview_result);
  
  *starmath_result = 0;
  sm_summ(summ, 0, starmath_result);
  debug(starmath_result);
  
  eq_transform((void**)&summ);
  debug(L"После eq_transform.");
  *starmath_result = 0;
  sm_summ(summ, 0, starmath_result);
  debug(starmath_result);
  
  eq_transform((void**)&summ);
  
  struct eq_leaf *number = get_summ_number(summ);
  if(number != NULL)
    wprintf(L"\nnumber = %f\n", number->value);
  
  *starmath_result = 0;
  sm_summ(summ, 0, starmath_result);
  debug(starmath_result);
  eq_node_delete(summ);
  
  return 0;
}
