#include <string.h>
#include <stdlib.h>
#include "eqtypes.h"
#include "common.h"


#define true  1
#define false 0


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
 * Clone node with children
 */
struct eq_node *eq_node_clone(struct eq_node *node)
{
  struct eq_node *clone = eq_node_new(node->type, node->sign);
  void *child = node->first_child;
  void *copy = NULL;
  void *(*eq_clone)(void *);
  
  if(child == NULL)
    return clone;
  
  eq_clone = get_clone_func(node->first_child);
  clone->first_child = eq_clone(node->first_child);
  copy = clone->first_child;
  
  while(((struct eq_node *)child)->next != NULL) {
    eq_clone = get_clone_func(((struct eq_node *)child)->next);
    
    ((struct eq_node *)copy)->next = eq_clone(((struct eq_node *)child)->next);
    
    copy  = ((struct eq_node *)copy)->next;
    child = ((struct eq_node *)child)->next;
  }
  
  return clone;
}

/*
 * Clone leaf
 */
struct eq_leaf *eq_leaf_clone(struct eq_leaf *leaf)
{
  struct eq_leaf *clone = eq_leaf_new(leaf->type, leaf->sign, leaf->value);
  return clone;
}

/*
 * Function returns pointer for one of clone functions
 */
void *(*(get_clone_func(void *equation)))(void *eq)
{
  if(((struct eq_node *)equation)->type == EQ_SYMBOL || ((struct eq_node *)equation)->type == EQ_NUMBER)
    return (void *((*)(void *)))eq_leaf_clone;
  else
    return (void *((*)(void *)))eq_node_clone;
}

/*
 * Delete equation node and free memory.
 * Returns pointer to "next" sibling.
 */
void *eq_node_delete(struct eq_node *node)
{
  void *next = node->next;
  void *(*delete)(void *);
  void *child = node->first_child;
  
  while(child != NULL) {
    delete = get_delete_func(child);
    child = delete(child);
  }
  free(node);
  
  return next;
}

/*
 * Delete equation leaf and free memory.
 * Returns pointer to "next" sibling.
 */
void *eq_leaf_delete(struct eq_leaf *leaf)
{
  void *next = leaf->next;
  free(leaf->value);
  free(leaf);
  return next;
}

/*
 * Function returns pointer for one of delete functions
 */
void *(*(get_delete_func(void *equation)))(void *)
{
  if(((struct eq_node *)equation)->type == EQ_SYMBOL || ((struct eq_node *)equation)->type == EQ_NUMBER)
    return (void *((*)(void *)))eq_leaf_delete;
  else
    return (void *((*)(void *)))eq_node_delete;
}

/* 
 * Compare two equations. If absolute is true, then sign is consider. 
 */
int eq_equals(struct eq_node *eq1, struct eq_node *eq2, int absolute)
{
  struct eq_node *child1;
  struct eq_node *child2;
  
  if(eq1->type != eq2->type || (absolute && eq1->sign != eq2->sign))
    return false;
  
  
  if(eq1->type == EQ_SYMBOL || eq1->type == EQ_NUMBER) {
    if(strcmp(((struct eq_leaf *)eq1)->value, ((struct eq_leaf *)eq2)->value) == 0)
      return true;
  } else {
    
    if(eq_children_count(eq1) != eq_children_count(eq2))
      return false;
    
    child1 = (struct eq_node *)eq1->first_child;
    child2 = (struct eq_node *)eq2->first_child;
    
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