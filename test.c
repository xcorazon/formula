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
  struct eq_node *head = eq_node_new(EQ_SUMM, 1);
  struct eq_node *summ = eq_node_new(EQ_SUMM, 1);
  //struct eq_node *mul  = eq_node_new(EQ_MUL, 1);
  struct eq_node *sinus  = eq_node_new(EQ_SIN, 1);
  struct eq_node *asinus  = eq_node_new(EQ_ASIN, -1);
  struct eq_leaf *a = eq_leaf_new(EQ_SYMBOL, 1, L"a", 0);
  struct eq_leaf *b = eq_leaf_new(EQ_NUMBER, 1, L"b", 180);
  //struct eq_leaf *c = eq_leaf_new(EQ_SYMBOL, -1, L"a_1", 0);
  //struct eq_leaf *b1 = eq_leaf_new(EQ_SYMBOL, 1, L"b_1", 20.345);

  setlocale(LC_ALL, "en_US.utf8");
  
  wprintf(L"cos(0.001) = %f, round = %i \n", cos(0.001), (int)round(cos(0.001)));
  
  wprintf(L"Size of wchar_t = %d\n", sizeof(wchar_t));
  
  summ->first_child = a;
  a->next = b;
  asinus->first_child = summ;
  sinus->first_child = asinus;
  head->first_child = sinus;
  
  wchar_t starmath_result[1000];
  wchar_t treeview_result[1000];
  
  *starmath_result = 0;
  sm_summ(head, SM_ROUND_BRACKET, starmath_result);
  wprintf(starmath_result);
  
  *treeview_result = 0;
  tree_view(head, SM_ROUND_BRACKET, treeview_result);
  wprintf(treeview_result);
  
  eq_calculate((void**)&head);
  
  *treeview_result = 0;
  tree_view(head, 0, treeview_result);
  debug(treeview_result);
  
  wprintf(L"\ntesting %f\n", 20.1);
  
  wprintf(L"\nresult: %s\n", treeview_result);
  
  eq_transform((void**)&head);
  wprintf(L"\nhead transformed...\n");  
  
  eq_combine_summ(head);
  eq_calculate_summ(&head);

  struct eq_node *multiplier = eq_find_common_mul(head);
  wprintf(L"\nfinded common multiplier...\n");
  if(multiplier != NULL) {
    eq_move_multiplier_out(&head, multiplier);
    wprintf(L"\nmultiplier moved out...\n");
  }
  *treeview_result = 0;
  tree_view(head, 0, treeview_result);
  wprintf(L"\nAfter move multiplier\n%s", treeview_result);
  
  eq_calculate((void**)&head);
  eq_calculate((void**)&head);
  
  tree_view(head, 0, treeview_result);
  wprintf(treeview_result);
  
  eq_transform((void**)&head);
  eq_transform((void**)&head);
  
  struct eq_leaf *number = get_summ_number(head);
  if(number != NULL)
    wprintf(L"\nnumber = %f\n", number->value);
  
  *treeview_result = 0;
  tree_view(head, 0, treeview_result);
  wprintf(treeview_result);
  eq_node_delete(head);
  
  return 0;
}
