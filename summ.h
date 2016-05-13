#ifndef SUMM_H
#define SUMM_H
#include "eqtypes.h"

struct eq_node *eq_add(void **node1, void **node2);
void eq_combine_summ(struct eq_node *node);

#endif /* SUMM_H */