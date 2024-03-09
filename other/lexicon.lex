%{
#include <stdio.h>
#include "y.tab.h"
#define ENTER_WORD_IN snprintf(yylval.word, sizeof(yylval.word) / sizeof(yylval.word[0]), "%s", yytext)

#ifdef ENABLE_LEX_TRACE
#define LOG_DEBUG(DATA) printf("%s: \"%s\"\n", DATA, yytext)
#else
#define LOG_DEBUG(DATA)
#endif

int lex_line = 1;

%}

%%
g_begin { LOG_DEBUG("GROUP_BEGIN"); return GROUP_BEGIN; }
g_end { LOG_DEBUG("GROUP_END"); return GROUP_END; }
if { LOG_DEBUG("IF"); return IF; }
elif { LOG_DEBUG("ELIF"); return ELIF; }
else { LOG_DEBUG("ELSE"); return ELSE; }
then { LOG_DEBUG("THEN"); return THEN; }
end { LOG_DEBUG("DELIMITER"); ENTER_WORD_IN; return DELIMITER; }
END { LOG_DEBUG("END"); ENTER_WORD_IN; return END; }
and  { LOG_DEBUG("CONJUNCTION"); ENTER_WORD_IN; return CONJUNCTION; }
of   { LOG_DEBUG("PROPOSITION"); ENTER_WORD_IN; return PROPOSITION; }
they { LOG_DEBUG("PRONOUN"); ENTER_WORD_IN; return PRONOUN; }
[A-Za-z]+o[s]? { LOG_DEBUG("NOUN"); ENTER_WORD_IN; return NOUN; }
[A-Za-z]+a { LOG_DEBUG("ADJECTIVE"); ENTER_WORD_IN; return ADJECTIVE; }
[A-Za-z]+as { LOG_DEBUG("VERB"); ENTER_WORD_IN; return VERB; }
[A-Za-z]+e { LOG_DEBUG("ADVERB"); ENTER_WORD_IN; return ADVERB; }
[1-9]000s { LOG_DEBUG("NUMBER_PLACE"); yylval.number = yytext[0] - '0'; return NUMBER_PLACE; }
[1-9]00s { LOG_DEBUG("NUMBER_100"); yylval.number = yytext[0] - '0'; return NUMBER_100; }
[1-9]0s { LOG_DEBUG("NUMBER_10"); yylval.number = yytext[0] - '0'; return NUMBER_10; }
[1-9]s { LOG_DEBUG("NUMBER_1"); yylval.number = yytext[0] - '0'; return NUMBER_1; }
\n { lex_line++; }
[[:space:]]+ ; /* Do nothing */
[^[:space:]]+  { printf("Lexer Error: \"%s\" is not a valid word in the language on line %d!\n", yytext, lex_line); exit(1); }
%%

int yywrap() {
    return 1;
}
