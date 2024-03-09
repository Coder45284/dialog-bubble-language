%{
#include <stdio.h>
#include "y.tab.h"
%}

%%
'\[' { return GROUP_BEGIN; }
'\]' { return GROUP_END; }
if { return IF; }
elif { return ELIF; }
else { return ELSE; }
then { return THEN; }
'\;' { snprintf(yylval.word, sizeof(yylval.word) / sizeof(yylval.word[0]), "%s", yytext); return DELIMITER; }
and  { snprintf(yylval.word, sizeof(yylval.word) / sizeof(yylval.word[0]), "%s", yytext); return CONJUNCTION; }
of   { snprintf(yylval.word, sizeof(yylval.word) / sizeof(yylval.word[0]), "%s", yytext); return PROPOSITION; }
they { snprintf(yylval.word, sizeof(yylval.word) / sizeof(yylval.word[0]), "%s", yytext); return PRONOUN; }
[A-Za-z]+o[s]? { snprintf(yylval.word, sizeof(yylval.word) / sizeof(yylval.word[0]), "%s", yytext); return NOUN; }
[A-Za-z]+a { snprintf(yylval.word, sizeof(yylval.word) / sizeof(yylval.word[0]), "%s", yytext); return ADJECTIVE; }
[A-Za-z]+as { snprintf(yylval.word, sizeof(yylval.word) / sizeof(yylval.word[0]), "%s", yytext); return VERB; }
[A-Za-z]+e { snprintf(yylval.word, sizeof(yylval.word) / sizeof(yylval.word[0]), "%s", yytext); return ADVERB; }
thousand { yylval.number = 1; return NUMBER_PLACE; }
100s { yylval.number = 100; return NUMBER_100; }
10s { yylval.number = 10; return NUMBER_10; }
1s { yylval.number = 1; return NUMBER_1; }
%%
