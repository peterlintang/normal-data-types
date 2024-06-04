/* Reverse polish notation calculator. */
%{
#define YYSTYPE double
#include <math.h>
int yylex (void);
void yyerror (char const *);
%}

%token NUM
%token ADD SUB MUL DIV POW NEG
%token EOL

%% /* Grammar rules and actions follow. */
input:    /* empty */
         | input line
;
line:    EOL
         | exp EOL { printf ("\t%.10g\n", $1); }
;
exp: 	 NUM { $$ = $1; }
	| exp exp ADD { $$ = $1 + $2; }
	| exp exp SUB { $$ = $1 - $2; }
	| exp exp MUL { $$ = $1 * $2; }
	| exp exp DIV { $$ = $1 / $2; }
	/* Exponentiation */
	| exp exp POW { $$ = pow ($1, $2); }
	/* Unary minus */
	| exp NEG { $$ = -$1; }
;
%%
