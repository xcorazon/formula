#ifndef STARMATH_H
#define STARMATH_H
#include "eqtypes.h"
#include <wchar.h>

#define SM_DEFAULT        0x00
#define SM_ROUND_BRACKET  0x01
#define SM_SHOW_SIGN     (0x01 << 1)

/// константы показывающие из какой функции пришел запрос
#define FR_SUMM   (1 << 16)
#define FR_MUL    (2 << 16)
#define FR_RECIP  (3 << 16)
#define FR_SIN    (4 << 16)
#define FR_COS    (5 << 16)

void sm_symbol(struct eq_node *node, int flags, wchar_t *result);
void sm_number(struct eq_node *node, int flags, wchar_t *result);
void sm_summ(struct eq_node *node, int flags, wchar_t *result);
void sm_mul(struct eq_node *node, int flags, wchar_t *result);

void sm_sincos(struct eq_node *node, int flags, wchar_t *result);
void sm_asincos(struct eq_node *node, int flags, wchar_t *result);

void sm_to_string(struct eq_node *node, int flags, wchar_t *result);

#endif /* STARMATH_H */
