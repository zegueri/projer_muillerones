#ifndef UTILITIES_H
#define UTILITIES_H

#include "models.h"

static functions *fun_list;

void init(void)
{
    fun_list = malloc(sizeof(functions));
    fun_list->n_fun = 0;
    fun_list->funs = malloc(sizeof(fun *) * MAX_FUN);
    if (fun_list->funs == NULL) {
        fprintf(stderr, "Failed to allocate memory for fun_list->funs\n");
    }
}

void create_fun(char *name, fun_vars vars, char **body)
{
    printf("Creating function '%s' with %d variables...\n", name, vars.n_vars);
    if (fun_list->funs == NULL) {
        fprintf(stderr, "fun_list->funs is not initialized. Call init() first.\n");
        exit(EXIT_FAILURE);
    }
    if (fun_list->n_fun >= MAX_FUN)
    {
        fprintf(stderr, "Maximum number of functions reached.\n");
        return;
    }
    fun *f = malloc(sizeof(fun));
    if (f == NULL) {
        fprintf(stderr, "Failed to allocate memory for fun\n");
        exit(EXIT_FAILURE);
    }
    f->name = name;
    f->vars = vars;
    f->body = body;

    printf("Cc ????\n");
    fun_list->funs[fun_list->n_fun] = f;
    fun_list->n_fun++;

    printf("Function '%s' created with %d variables.\n", name, vars.n_vars);
    printf("Variables: ");
    for (int i = 0; i < vars.n_vars; i++)
    {
        printf("%s ", vars.vars[i]);
    }
    printf("\n");
}

#endif