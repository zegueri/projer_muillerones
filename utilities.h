#ifndef UTILITIES_H
#define UTILITIES_H

#include "models.h"

extern functions *fun_list;

void init(void);
fun *find_fun(const char *name);
void list_functions(void);
void print_varlist(fun *f);

/* creation helpers */
void create_fun_formula(char *name, fun_vars vars, expr *e);
void create_fun_table(char *name, fun_vars vars, bool *table, int table_size);

/* evaluation utilities */
bool eval_fun(fun *f, bool *values);
void print_table(fun *f);

#endif
