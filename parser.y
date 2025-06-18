%{
extern int yylex(void);
extern void yyerror(const char*);
%}

%code requires {
    #include "utilities.h"

    #define N_VARS 8
}

%union { 
    bool boolean;
    char** body;
    fun_vars fv;
    char* string;
}

%token <string> NAME DEFINE LIST VARLIST EVAL AT TABLE FORMULA
%token <string> AND OR NOT XOR
%token <boolean> BOOL

%start s
%type <boolean> s command
%type <fv> vars vars_array
%type <body> formule_ou_table formula truth_table boolean_vars

%%
s : command {
        printf("On a une seule expression.\n");
    }
    | command s {
        printf("Encore une autre expression.\n");
    }
    | error s {
        yyerror("Erreur de syntaxe dans l'expression.");
        yyerrok; // On continue l'analyse après une erreur
    }
    ;

command : 
    DEFINE define;

define :
    NAME vars '=' formule_ou_table {
        create_fun($1, $2, $4);
    }

vars :
    %empty {
        printf("On a pas de variables spécifiées.\n");
        char* vars[8] = {"x", "y", "z", "s", "t", "u", "v", "w"};
        
        fun_vars fv;
        fv.n_vars = 8;
        fv.vars = vars;
        $$ = fv;
    }
    | '(' vars_array ')' {
        $$ = $2;
    }
    ;

vars_array :
    NAME {
        char *tab[8];
        tab[0] = $1;
        
        fun_vars fv;
        fv.n_vars = 1;
        fv.vars = tab;

        $$ = fv;
    }
    | vars_array ',' NAME {
        if ($1.n_vars >= 8) {
            yyerror("Trop de variables, max = 8.");
            return 1;
        }

        $1.n_vars++;
        $1.vars[$1.n_vars - 1] = $3;
        $$ = $1;
    }
    ;

// TODO : renommer wlh jsp comment appeler ça
formule_ou_table :
    formula {
        printf("On a une formule logique.\n");
    }
    | truth_table {
        printf("On a une table.\n");
    }
    ;

formula :
    BOOL {
        printf("On a une formule booléenne : %s\n", $1 ? "true" : "false");
    }
    | NAME {
        printf("On a une formule avec une variable : %s\n", $1);
    }
    expression {
        printf("On a une expression.\n");
    }
    | '(' formula ')' {
        printf("On a une formule entre parenthèses.\n");
    }

expression :
    NAME OR NAME {
        printf("On a une expression avec OR : %s OR %s\n", $1, $3);
    }
    | NAME AND NAME {
        printf("On a une expression avec AND : %s AND %s\n", $1, $3);
    }
    | NAME NOT {
        printf("On a une expression avec NOT : NOT %s\n", $2);
    }
    | NAME XOR NAME {
        printf("On a une expression avec XOR : %s XOR %s\n", $1, $3);
    }

truth_table :
    '{' boolean_vars '}' {
        printf("On a une table de vérité.\n");
    }

boolean_vars : 
    BOOL {}
    | boolean_vars ' ' BOOL
    | %empty
    ;

list :
    LIST {
        printf("TODO print liste de fonctions.\n");
    }

varlist :
    VARLIST {
        printf("TODO print liste de variables.\n");
    }

eval :
    EVAL NAME AT boolean_vars{
        printf("TODO évaluer la formule.\n");
    }

table :
    TABLE NAME {
        printf("TODO afficher la table de vérité pour %s.\n", $2);
    }
%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}