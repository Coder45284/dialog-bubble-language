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

int translateNumberWord(const char *word, char *prefix);

#include "pronouns.h"

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
(Wee)?(Tee)?(Qee)?(See)?Te[lh] {
    LOG_DEBUG("NUMBER_PLACE");

    char postfix = '@';
    int number = translateNumberWord(yytext, &postfix);
    double real_number;

    if(postfix == 'h')
        real_number = (double)((long long int)1 << ((number - 1) * 12));
    else
        real_number = 1. / ((long long int)1 << (number * 12));

    yylval.real_number = real_number;

    return NUMBER_PLACE;
}
(Weh)?(Teh)?(Qeh)?(Seh)?Tee {
    LOG_DEBUG("NUMBER_100");

    char postfix = '@';
    yylval.number = translateNumberWord(yytext, &postfix);

    return NUMBER_100;
}
(Wel)?(Tel)?(Qel)?(Sel)?Tee {
    LOG_DEBUG("NUMBER_10");

    char postfix = '@';
    yylval.number = translateNumberWord(yytext, &postfix);

    return NUMBER_10;
}
(Wee)?(Tee)?(Qee)?(See)?Tee {
    LOG_DEBUG("NUMBER_1");

    char postfix = '@';
    yylval.number = translateNumberWord(yytext, &postfix);

    return NUMBER_1;
}
[STWQ]ie([STWQ]el)?[SW]oe(Toe)? {
    LOG_DEBUG("PRONOUN");

    int person; // 4 possibilities
    int gender = 0; // 0 for unspecified. 5 possibilities.
    int type; // 2 possibilities
    int plurality = 0; // 0 for singlular, and 1 for plural.

    ENTER_WORD_IN;

    return NOUN;
}
[A-Za-z]+o[s]? { LOG_DEBUG("NOUN"); ENTER_WORD_IN; return NOUN; }
[A-Za-z]+a { LOG_DEBUG("ADJECTIVE"); ENTER_WORD_IN; return ADJECTIVE; }
[A-Za-z]+as { LOG_DEBUG("VERB"); ENTER_WORD_IN; return VERB; }
[A-Za-z]+e { LOG_DEBUG("ADVERB"); ENTER_WORD_IN; return ADVERB; }
\n { lex_line++; }
[ \t]+ ; /* Do nothing */
[^[:space:]]+  { printf("Lexer Error: \"%s\" is not a valid word in the language on line %d!\n", yytext, lex_line); exit(1); }
%%

int yywrap() {
    return 1;
}

int translateNumberWord(const char *word, char *prefix) {
    int number = 0;
    const char *word_head = word;
    const int PREFIXES[4] = {'W', 'T', 'Q', 'S'};
    const int NUMBERS[4]  = { 8,   4,   2,   1 };

    if(word_head[0] == '\0')
        return -1;

    for(int pre = 0; pre < 4; pre++) {
        if(word_head[0] == PREFIXES[pre]) {
            for(int i = 0; i < 3; i++) {
                word_head++;
                if(word_head[0] == '\0')
                    return -1;
            }

            number += NUMBERS[pre];
        }
    }

    if(word_head[0] != 'T')
        return -1;
    word_head++;
    if(word_head[0] != 'e')
        return -1;
    word_head++;
    if(word_head[0] == '\0')
        return -1;

    *prefix = word_head[0];

    return number;
}
