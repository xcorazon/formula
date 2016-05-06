#ifndef COMMON_H
#define COMMON_H
#include "eqtypes.h"

/*
 * Creation functions
 */
struct eq_node * eq_node_new(unsigned char type, char sign);
struct eq_leaf *eq_leaf_new(unsigned char type, char sign, char *value);

/*
 * Clone functions
 */
struct eq_node *eq_node_clone(struct eq_node *node);
struct eq_leaf *eq_leaf_clone(struct eq_leaf *leaf);
void *(*(get_clone_func(void *equation)))(void *eq);

/*
 * Delete functions
 */
void *eq_node_delete(struct eq_node *node);
void *eq_leaf_delete(struct eq_leaf *leaf);
void *(*(get_delete_func(void *equation)))(void *);

int eq_equals(struct eq_node *eq1, struct eq_node *eq2, int absolute);
int eq_children_count(struct eq_node *equation);

#endif /* COMMON_H */