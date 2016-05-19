#ifndef MUL_H
#define MUL_H
#include "eqtypes.h"

struct eq_node *eq_mul(void **node1, void **node2);
void eq_move_multiplier_out(struct eq_node **node, struct eq_node *multiplier);

#endif /* MUL_H */