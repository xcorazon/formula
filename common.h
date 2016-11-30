#ifndef COMMON_H
#define COMMON_H
#include "eqtypes.h"
#include <wchar.h>

/*
 * Creation functions
 */
struct eq_node *eq_node_new(unsigned char type, char sign);
struct eq_leaf *eq_leaf_new(unsigned char type, char sign, wchar_t *name, double val);

/*
 * Clone functions
 */
struct eq_node *eq_node_clone(struct eq_node *node);
struct eq_leaf *eq_leaf_clone(struct eq_leaf *leaf);

int eq_is_leaf(void *node);

void *(*(get_clone_func(void *equation)))(void *eq);
void *eq_clone(void * node);
/*
 * Delete functions
 */
void *eq_node_delete(struct eq_node *node);
void *eq_leaf_delete(struct eq_leaf *leaf);
void *eq_delete(void * node);
void *(*(get_delete_func(void *equation)))(void *);

int eq_equals(struct eq_node *eq1, struct eq_node *eq2, int absolute);
int eq_children_count(struct eq_node *equation);

void eq_move_children(struct eq_node *node1, struct eq_node *node2);
void eq_move_sign_in(struct eq_node *node);

void eq_remove_child(struct eq_node *node, struct eq_node *rem);
void *eq_find(struct eq_node *node, struct eq_node *equation);
void *eq_find_common_mul(struct eq_node *node);
void eq_connect_node(struct eq_node *parent, struct eq_node *node);

#endif /* COMMON_H */
