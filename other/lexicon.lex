%{
#include <stdio.h>
#include "y.tab.h"
#define ENTER_WORD_IN snprintf(yylval.word, sizeof(yylval.word) / sizeof(yylval.word[0]), "%s", yytext)

#ifndef ENABLE_LEX_TRACE
#define LOG_DEBUG(DATA) printf("%s: \"%s\"\n", DATA, yytext)
#else
#define LOG_DEBUG(DATA)
#endif

int lex_line = 1;

%}

%%
Sie { LOG_DEBUG("GROUP_BEGIN"); return GROUP_BEGIN; }
Soe { LOG_DEBUG("GROUP_END"); return GROUP_END; }
Tie { LOG_DEBUG("IF"); return IF; }
Teh { LOG_DEBUG("ELIF"); return ELIF; }
Tee { LOG_DEBUG("ELSE"); return ELSE; }
Toe { LOG_DEBUG("THEN"); return THEN; }
See { LOG_DEBUG("DELIMITER END"); ENTER_WORD_IN; return DELIMITER; }
Qee { LOG_DEBUG("DELIMITER END | Yes/No Question"); ENTER_WORD_IN; return DELIMITER; }
END { LOG_DEBUG("END"); ENTER_WORD_IN; return END; }
and  { LOG_DEBUG("CONJUNCTION"); ENTER_WORD_IN; return CONJUNCTION; }
of   { LOG_DEBUG("PROPOSITION 'of'"); ENTER_WORD_IN; return PROPOSITION; }
they { LOG_DEBUG("PRONOUN"); ENTER_WORD_IN; return PRONOUN; }
[A-Za-z]+o[s]? { LOG_DEBUG("NOUN"); ENTER_WORD_IN; return NOUN; }
[A-Za-z]+a { LOG_DEBUG("ADJECTIVE"); ENTER_WORD_IN; return ADJECTIVE; }
[A-Za-z]+as { LOG_DEBUG("VERB"); ENTER_WORD_IN; return VERB; }
[A-Za-z]+e { LOG_DEBUG("ADVERB"); ENTER_WORD_IN; return ADVERB; }
\+ { LOG_DEBUG("NUMBER_SIGN"); yylval.number =  1; return NUMBER_SIGN; }
\- { LOG_DEBUG("NUMBER_SIGN"); yylval.number = -1; return NUMBER_SIGN; }
[0-9][bl] {
    LOG_DEBUG("NUMBER_PLACE");

    int number = yytext[0] - '0';

    if(yytext[1] == 'b')
        yylval.real_number = ((long long int)1 << (number * 12));
    else
        yylval.real_number = 1. / ((long long int)1 << ((number + 1) * 12));

    return NUMBER_PLACE;
}
[1-9a-f]00s {
    LOG_DEBUG("NUMBER_100");
    if(yytext[0] >= 'a')
        yylval.number = (yytext[0] - 'a') + 10;
    else
        yylval.number = (yytext[0] - '0');
    return NUMBER_100;
}
[1-9a-f]0s {
    LOG_DEBUG("NUMBER_10");
    if(yytext[0] >= 'a')
        yylval.number = (yytext[0] - 'a') + 10;
    else
        yylval.number = (yytext[0] - '0');
    return NUMBER_10;
}
[0-9a-f]s {
    LOG_DEBUG("NUMBER_1");
    if(yytext[0] >= 'a')
        yylval.number = (yytext[0] - 'a') + 10;
    else
        yylval.number = (yytext[0] - '0');
    return NUMBER_1;
}
\n { lex_line++; }
[[:space:]]+ ; /* Do nothing */
[^[:space:]]+  { printf("Lexer Error: \"%s\" is not a valid word in the language on line %d!\n", yytext, lex_line); exit(1); }
%%

int yywrap() {
    return 1;
}
