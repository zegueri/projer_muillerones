#include "utilities.h"
#include <string.h>

functions *fun_list = NULL;

void init(void)
{
    fun_list = malloc(sizeof(functions));
    if (!fun_list) {
        fprintf(stderr, "Failed to allocate memory for fun_list\n");
        exit(EXIT_FAILURE);
    }
    fun_list->n_fun = 0;
    fun_list->funs = malloc(sizeof(fun *) * MAX_FUN);
    if (!fun_list->funs) {
        fprintf(stderr, "Failed to allocate memory for fun_list->funs\n");
        exit(EXIT_FAILURE);
    }
}

fun *find_fun(const char *name)
{
    if (!fun_list) return NULL;
    for (int i = 0; i < fun_list->n_fun; ++i) {
        if (strcmp(fun_list->funs[i]->name, name) == 0)
            return fun_list->funs[i];
    }
    return NULL;
}

void list_functions(void)
{
    if (!fun_list) return;
    for (int i = 0; i < fun_list->n_fun; ++i)
        printf("%s ", fun_list->funs[i]->name);
    printf("\n");
}

void print_varlist(fun *f)
{
    if (!f) return;
    for (int i = 0; i < f->vars.n_vars; ++i)
        printf("%s ", f->vars.vars[i]);
    printf("\n");
}

static bool eval_expr(expr *e, char **vars, bool *values, int n)
{
    switch (e->type) {
    case EXPR_CONST:
        return e->u.constant;
    case EXPR_VAR:
        for (int i = 0; i < n; ++i)
            if (strcmp(vars[i], e->u.var) == 0)
                return values[i];
        return false;
    case EXPR_NOT:
        return !eval_expr(e->u.unary, vars, values, n);
    case EXPR_AND:
        return eval_expr(e->u.bin.left, vars, values, n) &&
               eval_expr(e->u.bin.right, vars, values, n);
    case EXPR_OR:
        return eval_expr(e->u.bin.left, vars, values, n) ||
               eval_expr(e->u.bin.right, vars, values, n);
    case EXPR_XOR:
        return eval_expr(e->u.bin.left, vars, values, n) ^
               eval_expr(e->u.bin.right, vars, values, n);
    case EXPR_IMPL:
        return !eval_expr(e->u.bin.left, vars, values, n) ||
               eval_expr(e->u.bin.right, vars, values, n);
    }
    return false;
}

static void fill_table(fun *f)
{
    if (f->body.type != BODY_FORMULA) return;
    int size = 1 << f->vars.n_vars;
    expr *form = f->body.u.formula;
    bool *table = malloc(sizeof(bool) * size);
    bool values[MAX_VARS];
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < f->vars.n_vars; ++j)
            values[j] = (i >> (f->vars.n_vars - j - 1)) & 1;
        table[i] = eval_expr(form, f->vars.vars, values, f->vars.n_vars);
    }
    f->body.type = BODY_TABLE;
    f->body.u.table = table;
    /* keep formula for now? could free form */
}

void create_fun_formula(char *name, fun_vars vars, expr *e)
{
    if (!fun_list || fun_list->n_fun >= MAX_FUN) return;
    fun *f = malloc(sizeof(fun));
    f->name = strdup(name);
    f->vars.n_vars = vars.n_vars;
    f->vars.vars = malloc(sizeof(char*) * vars.n_vars);
    for (int i = 0; i < vars.n_vars; ++i)
        f->vars.vars[i] = strdup(vars.vars[i]);
    f->body.type = BODY_FORMULA;
    f->body.u.formula = e;
    fun_list->funs[fun_list->n_fun++] = f;
    fill_table(f);
}

void create_fun_table(char *name, fun_vars vars, bool *table, int table_size)
{
    if (!fun_list || fun_list->n_fun >= MAX_FUN) return;
    fun *f = malloc(sizeof(fun));
    f->name = strdup(name);
    f->vars.n_vars = vars.n_vars;
    f->vars.vars = malloc(sizeof(char*) * vars.n_vars);
    for (int i = 0; i < vars.n_vars; ++i)
        f->vars.vars[i] = strdup(vars.vars[i]);
    f->body.type = BODY_TABLE;
    int size = 1 << vars.n_vars;
    f->body.u.table = malloc(sizeof(bool) * size);
    for (int i = 0; i < size; ++i)
        f->body.u.table[i] = table[i];
    fun_list->funs[fun_list->n_fun++] = f;
}

bool eval_fun(fun *f, bool *values)
{
    if (!f) return false;
    if (f->body.type == BODY_FORMULA)
        return eval_expr(f->body.u.formula, f->vars.vars, values, f->vars.n_vars);
    int idx = 0;
    for (int i = 0; i < f->vars.n_vars; ++i) {
        idx = (idx << 1) | (values[i] ? 1 : 0);
    }
    return f->body.u.table[idx];
}

void print_table(fun *f)
{
    if (!f) return;
    int size = 1 << f->vars.n_vars;
    for (int i = 0; i < size; ++i)
        printf("%d ", f->body.u.table[i]);
    printf("\n");
}


