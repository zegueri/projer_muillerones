#ifndef UTILITIES_H
#define UTILITIES_H

#include "models.h"

extern functions *fun_list;

void init(void);
void create_fun(char *name, fun_vars vars, char **body);

#endif
