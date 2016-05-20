#ifndef MUL_H
#define MUL_H
#include "eqtypes.h"

struct eq_node *eq_mul(void **node1, void **node2);
void eq_move_multiplier_out(struct eq_node **node, struct eq_node *multiplier);
void eq_combine_mul(struct eq_node *node);
void eq_calculate_mul(struct eq_node *node, void (*calculate)(void *));

#endif /* MUL_H */