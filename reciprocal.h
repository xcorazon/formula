#ifndef RECIPROCAL_H
#define RECIPROCAL_H

#include "eqtypes.h"
void eq_combine_recip(struct eq_node *mul);
void eq_unwrap_recip(struct eq_node **rec);
void eq_transform_reciprocal(void **rec, void (*transform)(void **));
void eq_calculate_reciprocal(struct eq_node **node, void (*calculate)(void **));

#endif /*RECIPROCAL_H*/