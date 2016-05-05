#ifndef COMMON_H
#define COMMON_H
#include "eqtypes.h"

struct eq_node * eq_node_new(unsigned char type, char sign);
struct eq_leaf *eq_leaf_new(unsigned char type, char sign, char *value);

int eq_equals(struct eq_node *eq1, struct eq_node *eq2, int absolute);
int eq_children_count(struct eq_node *equation);

#endif /* COMMON_H */