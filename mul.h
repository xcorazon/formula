#ifndef MUL_H
#define MUL_H
#include "eqtypes.h"

struct eq_node *eq_mul(void **node1, void **node2);
void eq_move_multiplier_out(struct eq_node **node, struct eq_node *multiplier);
void eq_move_multiplier_in(struct eq_node **node);
void eq_combine_mul(struct eq_node *node);
void eq_calculate_mul(struct eq_node **node);
void div_symbols(struct eq_node *mul);
void eq_transform_mul(void **mul);

void eq_move_multipliers_out(struct eq_node **node);

#endif /* MUL_H */