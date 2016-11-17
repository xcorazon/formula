#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
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
struct eq_leaf *eq_leaf_new(unsigned char type, char sign, char *name, double val)
{
  if(type != EQ_SYMBOL && type != EQ_NUMBER)
    return NULL;
  
  struct eq_leaf *leaf = (struct eq_leaf *)malloc(sizeof(*leaf));
  if(type == EQ_SYMBOL) {
    wchar_t *str = (wchar_t *)malloc((wcslen(name) + 1)*sizeof(wchar_t));
  
    wcscpy(str, name);
    leaf->name = str;
  } else
    leaf->value = val;

  
  leaf->type = type;
  if(sign >= 0)
    leaf->sign = 1;
  else
    leaf->sign = -1;
  leaf->next = NULL;
  
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
  struct eq_leaf *clone = eq_leaf_new(leaf->type, leaf->sign, leaf->name, leaf->value);
  return clone;
}

inline int eq_is_leaf(void *node)
{
  return (((struct eq_node *)node)->type == EQ_SYMBOL || ((struct eq_node *)node)->type == EQ_NUMBER);
}

/*
 * Function returns pointer for one of clone functions
 */
void *(*(get_clone_func(void *equation)))(void *eq)
{
  if(eq_is_leaf(equation))
    return (void *((*)(void *)))eq_leaf_clone;
  else
    return (void *((*)(void *)))eq_node_clone;
}

void *eq_clone(void * node)
{
  if(eq_is_leaf(node))
    return eq_leaf_clone(node);
  else
    return eq_node_clone(node);
}

/*
 * Delete equation node and free memory.
 * Returns pointer to "next" sibling.
 */
void *eq_node_delete(struct eq_node *node)
{
  if(node == NULL)
    return NULL;
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
  if(leaf == NULL)
    return NULL;
  void *next = leaf->next;
  if(leaf->type == EQ_SYMBOL)
    free(leaf->name);
  free(leaf);

  return next;
}

void *eq_delete(void * node)
{
  if(node == NULL)
    return NULL;
  if(eq_is_leaf(node))
    return eq_leaf_delete((struct eq_leaf *)node);
  else
    return eq_node_delete((struct eq_node *)node);
}

/*
 * Function returns pointer for one of delete functions
 */
void *(*(get_delete_func(void *equation)))(void *)
{
  if(eq_is_leaf(equation))
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
  
  void *children[100];
  for(int i=0; i<100; i++)
    children[i] = NULL;
  int next_child = 0;
  
  if(eq1->type != eq2->type || (absolute && eq1->sign != eq2->sign))
    return false;
  
  
  if(eq_is_leaf(eq1)) {
    
    if(eq1->type == EQ_NUMBER && ((struct eq_leaf *)eq1)->value == ((struct eq_leaf *)eq2)->value)
      return true;
    else if(wcscmp(((struct eq_leaf *)eq1)->name, ((struct eq_leaf *)eq2)->name) == 0)
      return true;
  } else {
    
    if(eq_children_count(eq1) != eq_children_count(eq2))
      return false;
    
    child1 = (struct eq_node *)eq1->first_child;
    
    while (child1 != NULL) {
      int match_finded = false;
      child2 = (struct eq_node *)eq2->first_child;
      
      while(child2 != NULL) {
        
        int child_in_array = false;
        for(int i=0; i<next_child; i++) {
          if(children[i] == child2) {
            child_in_array = true;
            break;
          }
        }
        
        if(!child_in_array && eq_equals(child1, child2, true)) {
          children[next_child] = child2;
          next_child++;
          match_finded = true;
          break;
        }
        child2 = child2->next;
      }
      
      if(!match_finded)
        return false;
      
      child1 = child1->next;
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

/*
 * Move children from node2 to node1. 
 */
void eq_move_children(struct eq_node *node1, struct eq_node *node2)
{
  struct eq_node *child = node1->first_child;
  if(child == NULL)
    return;
  
  while(child->next != NULL)
    child = child->next;
  
  if(node2->type == EQ_SUMM) {
    eq_move_sign_in(node2);
    child->next = node2->first_child;
    node2->first_child = NULL;
  } else if(node2->type == EQ_MUL || node2->type == EQ_RECIPROCAL) {
    ((struct eq_node *)node2->first_child)->sign *= node2->sign;
    child->next = node2->first_child;
    node2->first_child = NULL;
  } else {
    child->next = node2;
  }
}

/*
 * Move sign in the brackets.
 */
void eq_move_sign_in(struct eq_node *node)
{
  struct eq_node *child = node->first_child;
  if( node->type == EQ_SUMM) {
    while(child != NULL) {
      child->sign *= node->sign;
      child = (struct eq_node *)child->next;
    }
    node->sign = 1;
  }
}

/*
 * Removes one equal child from node
 */
void eq_remove_child(struct eq_node *node, struct eq_node *rem)
{
  struct eq_node *child = node->first_child;
  struct eq_node **prev = (struct eq_node **)&node->first_child;
  
  while(child != NULL) {
    if(eq_equals(child, rem, false)) {
      if(node->type == EQ_MUL)
        node->sign *= child->sign * rem->sign;
      *prev = eq_delete(child);
      break;
    }
    prev = (struct eq_node **)&child->next;
    child = child->next;
  }
}

/*
 * Find equation.
 */
void *eq_find(struct eq_node *node, struct eq_node *equation)
{
  if(eq_is_leaf(node))
    goto ret;
  
  struct eq_node *child = node->first_child;
  while(child != NULL) {
    if(child->type == EQ_MUL) {
      if(eq_find(child, equation) != NULL)
        return child;
    }
    if(eq_equals(child, equation, false)) 
      return child;
    child = child->next;
  }
ret:
  return NULL;
}

/*
 * Find common multiplier.
 */
void *eq_find_common_mul(struct eq_node *node)
{
  if(node->type != EQ_SUMM)
    goto ret;
  
  struct eq_node *child = node->first_child;
  struct eq_node *current = child->next;
  struct eq_node *result;
  
  while(child->next != NULL) {
    if(child->type == EQ_MUL)
      result = child->first_child;
    else
      result = child;
    
    while(result != NULL) {
      while(current != NULL) {
        if(eq_equals(result, current, false) || (current->type == EQ_MUL && eq_find(current, result) != NULL))
          return result;
        current = current->next;
      }
      
      if(child->type == EQ_MUL) {
        current = child->next;
        result  = result->next;
      } else {
        child   = child->next;
        current = child->next;
        break;
      }
    }
  }
ret:
  return NULL;
}
