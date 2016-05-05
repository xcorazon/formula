#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "eqtypes.h"
#include "common.h"


/*
 * Creates node of equation tree.
 */
struct eq_node *eq_node_new(unsigned char type, char sign)
{
  struct eq_node *node = (struct eq_node *)malloc(sizeof(*node));
  
  node->type = type;
  if(sign >= 0)
    node->sign = 1;
  else
    node->sign = -1;
  
  node->next = NULL;
  node->first_child = NULL;
  
  return node;
}

/*
 * Creates Number or Symbol
 */
struct eq_leaf *eq_leaf_new(unsigned char type, char sign, char *value)
{
  struct eq_leaf *leaf = (struct eq_leaf *)malloc(sizeof(*leaf));
  char *str = (char *)malloc(strlen(value) + 1);
  
  strcpy(str, value);
  
  leaf->type = type;
  if(sign >= 0)
    leaf->sign = 1;
  else
    leaf->sign = -1;
  leaf->next = NULL;
  leaf->value = str;
  
  return leaf;
}

/* 
 * Compare two equations. If absolute is true, then sign is consider. 
 */
int eq_equals(struct eq_node *eq1, struct eq_node *eq2, int absolute)
{
  
  if(eq1->type != eq2->type || (absolute && eq1->sign != eq2->sign))
    return false;
  
  
  if(eq1->type == MATH_SYMBOL || eq1->type == MATH_NUMBER) {
    if(strcmp((eq_leaf *)eq1->value, (eq_leaf *)eq2->value) == 0)
      return true;
  } else {
    
    if(eq_children_count(eq1) != eq_children_count(eq2))
      return false;
    
    struct eq_node *child1 = eq1->first_child;
    struct eq_node *child2 = eq2->first_child;
    
    while (child1 != NULL){
      if(!eq_equals(child1, child2, true))
        return false;
      child1 = child1->next;
      child2 = child2->next;
    }
    return true;
  }
  
  return false;
}


/*
 * Get count of children of node. 
 */
int eq_children_count(struct eq_node *equation)
{
  int count = 0;
  struct eq_node *node = equation->first_child;
  
  if(node == NULL)
    goto ret;
  
  count++;
  while(node->next != NULL) {
    node = node->next;
    count++;
  }
  
ret:
  return count;
}