#ifndef EQ_TYPES_H
#define EQ_TYPES_H

enum eq_type {
    EQ_SYMBOL = 1,
    EQ_NUMBER,
    EQ_SUMM,
    EQ_MUL,
    EQ_RECIPROCAL,
    EQ_SIN,
    EQ_COS,
    EQ_ASIN,
    EQ_ACOS,
    EQ_POW
};

/* Node of equation tree */
struct eq_node {
    unsigned char type;
    char sign;
    void *next;
    
    void *first_child;
};

/* Leaf of equation tree */
struct eq_leaf {
    unsigned char type;
    char sign;
    void *next;
    
    union {
      char *name;
      double value;
    };
    unsigned char priority;
};

#endif /* EQ_TYPES_H */