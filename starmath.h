#ifndef STARMATH_H
#define STARMATH_H
#include "eqtypes.h"

void sm_symbol(struct eq_node *node, char *result);
void sm_number(struct eq_node *node, char *result);
void sm_summ(struct eq_node *node, char *result);

void sm_to_string(struct eq_node *node, char *result);

#endif /* STARMATH_H */