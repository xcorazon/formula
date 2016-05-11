#ifndef STARMATH_H
#define STARMATH_H
#include "eqtypes.h"

#define SM_DEFAULT        0x00
#define SM_ROUND_BRACKET  0x01
#define SM_SHOW_SIGN     (0x01 << 1)

void sm_symbol(struct eq_node *node, int flags, char *result);
void sm_number(struct eq_node *node, int flags, char *result);
void sm_summ(struct eq_node *node, int flags, char *result);

void sm_to_string(struct eq_node *node, int flags, char *result);

#endif /* STARMATH_H */