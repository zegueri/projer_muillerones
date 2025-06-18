%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utilities.h"

extern int yylex(void);
extern void yyerror(const char *s);

static int bool_list_len(bool *b) {
    int n=0; while(b[n]!=-1) n++; return n;
}
%}

%code requires {
    #include "models.h"
    #include <stdbool.h>
}

%union {
    bool boolean;
    char *string;
    expr *exprptr;
    bool *boollist;
    fun_vars fv;
}

%token <string> NAME
%token <boolean> BOOL
%token DEFINE LIST VARLIST EVAL AT TABLE FORMULA
%token AND OR NOT XOR IMPL
%left OR
%left XOR
%left AND
%right NOT
%right IMPL
%type <exprptr> formula expr
%type <boollist> bool_list
%type <fv> opt_vars var_names

%%
input:
    /* empty */
  | input command
  ;

command:
      DEFINE NAME opt_vars '=' formula       { create_fun_formula($2, $3, $5); }
    | DEFINE NAME opt_vars '=' '{' bool_list '}' { create_fun_table($2, $3, $6, bool_list_len($6)); }
    | LIST                                   { list_functions(); }
    | VARLIST NAME                           { fun *f=find_fun($2); if(f) print_varlist(f); else printf("unknown\n"); }
    | EVAL NAME AT bool_list                 {
          fun *f=find_fun($2);
          if(f){
              int n=bool_list_len($4);
              bool vals[MAX_VARS]={0};
              for(int i=0;i<n && i<MAX_VARS; i++) vals[i]=$4[i];
              printf("%d\n", eval_fun(f, vals));
          }
      }
    | TABLE NAME                             { fun *f=find_fun($2); if(f) print_table(f); }
    ;

opt_vars:
      '(' var_names ')' { $$ = $2; }
    | /* empty */      { fun_vars fv; fv.n_vars=0; fv.vars=NULL; $$=fv; }
    ;

var_names:
      NAME               { fun_vars fv; fv.n_vars=1; fv.vars=malloc(sizeof(char*)); fv.vars[0]=strdup($1); $$=fv; }
    | var_names ',' NAME { $1.n_vars++; $1.vars=realloc($1.vars,sizeof(char*)*$1.n_vars); $1.vars[$1.n_vars-1]=strdup($3); $$=$1; }
    ;

bool_list:
      /* empty */        { bool *b=malloc(sizeof(bool)); b[0]=-1; $$=b; }
    | BOOL              { bool *b=malloc(2*sizeof(bool)); b[0]=$1; b[1]=-1; $$=b; }
    | bool_list BOOL    {
          int n=0; while($1[n]!=-1) n++; $1=realloc($1,(n+2)*sizeof(bool)); $1[n]=$2; $1[n+1]=-1; $$=$1; }
    ;

formula:
      expr                { $$ = $1; }
    ;

expr:
      BOOL                { expr *e=malloc(sizeof(expr)); e->type=EXPR_CONST; e->u.constant=$1; $$=e; }
    | NAME                { expr *e=malloc(sizeof(expr)); e->type=EXPR_VAR; e->u.var=strdup($1); $$=e; }
    | NOT expr            { expr *e=malloc(sizeof(expr)); e->type=EXPR_NOT; e->u.unary=$2; $$=e; }
    | expr AND expr       { expr *e=malloc(sizeof(expr)); e->type=EXPR_AND; e->u.bin.left=$1; e->u.bin.right=$3; $$=e; }
    | expr OR expr        { expr *e=malloc(sizeof(expr)); e->type=EXPR_OR; e->u.bin.left=$1; e->u.bin.right=$3; $$=e; }
    | expr XOR expr       { expr *e=malloc(sizeof(expr)); e->type=EXPR_XOR; e->u.bin.left=$1; e->u.bin.right=$3; $$=e; }
    | expr IMPL expr      { expr *e=malloc(sizeof(expr)); e->type=EXPR_IMPL; e->u.bin.left=$1; e->u.bin.right=$3; $$=e; }
    | '(' expr ')'        { $$=$2; }
    ;
%%

void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}
