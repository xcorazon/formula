#ifndef SINCOS_H
#define SINCOS_H

#include "eqtypes.h"

void eq_calculate_sin(struct eq_node **node);
void eq_transform_sin(void **node);
void eq_calculate_cos(struct eq_node **node);
void eq_transform_cos(void **node);

#endif /*SINCOS_H*/