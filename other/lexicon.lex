%{
#include <stdio.h>
#include "y.tab.h"
#define ENTER_WORD_IN snprintf(yylval.word, sizeof(yylval.word) / sizeof(yylval.word[0]), "%s", yytext)
%}

%%
gb { return GROUP_BEGIN; }
ge { return GROUP_END; }
if { return IF; }
elif { return ELIF; }
else { return ELSE; }
then { return THEN; }
end { ENTER_WORD_IN; return DELIMITER; }
and  { ENTER_WORD_IN; return CONJUNCTION; }
of   { ENTER_WORD_IN; return PROPOSITION; }
they { ENTER_WORD_IN; return PRONOUN; }
[A-Za-z]+o[s]? { ENTER_WORD_IN; return NOUN; }
[A-Za-z]+a { ENTER_WORD_IN; return ADJECTIVE; }
[A-Za-z]+as { ENTER_WORD_IN; return VERB; }
[A-Za-z]+e { ENTER_WORD_IN; return ADVERB; }
thousand { yylval.number = 1; return NUMBER_PLACE; }
100s { yylval.number = 100; return NUMBER_100; }
10s { yylval.number = 10; return NUMBER_10; }
1s { yylval.number = 1; return NUMBER_1; }
[[:space:]]+ ; /* Do nothing */
.+  { printf("Error: \"%s\" is not a valid word in the language on line %d!\n", yytext, yylineno); exit(1); }
%%

int yywrap() {
    return 1;
}
