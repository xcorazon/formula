#ifndef STARMATH_H
#define STARMATH_H
#include "eqtypes.h"

#define SM_DEFAULT        0x00
#define SM_ROUND_BRACKET  0x01
#define SM_SHOW_SIGN     (0x01 << 1)

/// константы показывающие из какой функции пришел запрос
#define FR_SUMM   (1 << 16)
#define FR_MUL    (2 << 16)
#define FR_RECIP  (3 << 16)
#define FR_SIN    (4 << 16)
#define FR_COS    (5 << 16)

void sm_symbol(struct eq_node *node, int flags, char *result);
void sm_number(struct eq_node *node, int flags, char *result);
void sm_summ(struct eq_node *node, int flags, char *result);
void sm_mul(struct eq_node *node, int flags, char *result);
void sm_reciprocal(struct eq_node *node, int flags, char *result);

void sm_sin(struct eq_node *node, int flags, char *result);
void sm_cos(struct eq_node *node, int flags, char *result);
void sm_asin(struct eq_node *node, int flags, char *result);
void sm_acos(struct eq_node *node, int flags, char *result);

void sm_to_string(struct eq_node *node, int flags, char *result);

#endif /* STARMATH_H */