#include "utilities.h"

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

void create_fun(char *name, fun_vars vars, char **body)
{
    printf("Creating function '%s' with %d variables...\n", name, vars.n_vars);
    if (!fun_list) {
        fprintf(stderr, "fun_list is not initialized. Call init() first.\n");
        exit(EXIT_FAILURE);
    }
    if (fun_list->n_fun >= MAX_FUN) {
        fprintf(stderr, "Maximum number of functions reached.\n");
        return;
    }
    fun *f = malloc(sizeof(fun));
    if (!f) {
        fprintf(stderr, "Failed to allocate memory for fun\n");
        exit(EXIT_FAILURE);
    }
    f->name = name;
    f->vars = vars;
    f->body = body;
    fun_list->funs[fun_list->n_fun++] = f;
}


