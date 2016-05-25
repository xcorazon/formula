#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "eqtypes.h"

void eq_calculate(void *node);
void eq_transform(void **node);
void eq_transform_children(struct eq_node *node, void (*transform)(void **));

#endif /* TRANSFORMATIONS_H */