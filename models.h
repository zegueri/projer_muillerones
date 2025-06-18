#ifndef MODELS_H
#define MODELS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_FUN 20

typedef struct
{
    int n_vars;
    char **vars;
} fun_vars;

typedef struct
{
    char *name;
    fun_vars vars;
    char** body;
} fun;

typedef struct
{
    int n_fun;
    fun **funs;
} functions;

#endif