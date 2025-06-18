#ifndef MODELS_H
#define MODELS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_FUN 20
#define MAX_VARS 8

typedef enum {
    BODY_FORMULA,
    BODY_TABLE
} body_type;

/* Expression node for logical formulas */
typedef struct expr {
    enum { EXPR_VAR, EXPR_CONST, EXPR_NOT, EXPR_AND, EXPR_OR, EXPR_XOR, EXPR_IMPL } type;
    union {
        char *var;            /* variable name for EXPR_VAR */
        bool constant;        /* value for EXPR_CONST */
        struct expr *unary;   /* child for NOT */
        struct { struct expr *left, *right; } bin; /* binary operators */
    } u;
} expr;

typedef struct {
    int n_vars;          /* number of variables */
    char **vars;         /* array of variable names */
} fun_vars;

typedef struct {
    body_type type;      /* BODY_FORMULA or BODY_TABLE */
    union {
        expr *formula;   /* root of expression tree */
        bool *table;     /* truth table, size 2^n_vars */
    } u;
} fun_body;

typedef struct {
    char *name;          /* function name */
    fun_vars vars;       /* variable list */
    fun_body body;       /* representation */
} fun;

typedef struct {
    int n_fun;           /* number of functions stored */
    fun **funs;          /* array of pointers to functions */
} functions;

#endif