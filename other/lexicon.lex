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
Seh { LOG_DEBUG("CONJUNCTION 'and"); ENTER_WORD_IN; return CONJUNCTION; }
Sel { LOG_DEBUG("CONJUNCTION 'or'"); ENTER_WORD_IN; return CONJUNCTION; }
Tel { LOG_DEBUG("CONJUNCTION 'because'"); ENTER_WORD_IN; return CONJUNCTION; }
Weh { LOG_DEBUG("CONJUNCTION 'so'"); ENTER_WORD_IN; return CONJUNCTION; }
SelSel { LOG_DEBUG("PROPOSITION 'similar to'"); ENTER_WORD_IN; return PROPOSITION; }
TelSeh { LOG_DEBUG("PROPOSITION 'because'"); ENTER_WORD_IN; return PROPOSITION; }
TeeWel { LOG_DEBUG("PROPOSITION 'using'"); ENTER_WORD_IN; return PROPOSITION; }
QeeTelQee { LOG_DEBUG("PROPOSITION 'to'"); ENTER_WORD_IN; return PROPOSITION; }
TelQeeTel { LOG_DEBUG("PROPOSITION 'from'"); ENTER_WORD_IN; return PROPOSITION; }
QehTel { LOG_DEBUG("PROPOSITION 'on|at'"); ENTER_WORD_IN; return PROPOSITION; }
QelSee { LOG_DEBUG("PROPOSITION 'in/inside of'"); ENTER_WORD_IN; return PROPOSITION; }
WelQeeQee { LOG_DEBUG("PROPOSITION 'for'"); ENTER_WORD_IN; return PROPOSITION; }
SehWee { LOG_DEBUG("PROPOSITION 'because of'"); ENTER_WORD_IN; return PROPOSITION; }
SeeSee { LOG_DEBUG("PROPOSITION 'of'"); ENTER_WORD_IN; return PROPOSITION; }
TeeWelTee { LOG_DEBUG("NUMBER_SIGN"); yylval.number =  1; return NUMBER_SIGN; }
SeeWehTee { LOG_DEBUG("NUMBER_SIGN"); yylval.number = -1; return NUMBER_SIGN; }
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
they { LOG_DEBUG("PRONOUN"); ENTER_WORD_IN; return PRONOUN; }
[A-Za-z]+o[s]? { LOG_DEBUG("NOUN"); ENTER_WORD_IN; return NOUN; }
[A-Za-z]+a { LOG_DEBUG("ADJECTIVE"); ENTER_WORD_IN; return ADJECTIVE; }
[A-Za-z]+as { LOG_DEBUG("VERB"); ENTER_WORD_IN; return VERB; }
[A-Za-z]+e { LOG_DEBUG("ADVERB"); ENTER_WORD_IN; return ADVERB; }
\n { lex_line++; }
[[:space:]]+ ; /* Do nothing */
[^[:space:]]+  { printf("Lexer Error: \"%s\" is not a valid word in the language on line %d!\n", yytext, lex_line); exit(1); }
%%

int yywrap() {
    return 1;
}
