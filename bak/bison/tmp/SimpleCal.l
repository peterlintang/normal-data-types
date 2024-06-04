%{
#include "SimpleCal.tab.h"
%}
%%
"+"     { return ADD; }
"-"     { return SUB; }
"*"     { return MUL; }
"/"     { return DIV; }
 
"|"     {return ABS; }
^[-+][0-9]+ { yylval = atoi(yytext); return NUMBER;}
[0-9]+  { yylval = atoi(yytext); return NUMBER;}
 
\n {return EOL; }
[ \t]   {} 
.       { yyerror("Mystery character: %c!",*yytext);}
        /*这里输出有点问题...*/
%%
