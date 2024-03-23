%{
#include <stdio.h>
#include "y.tab.h"

char lexer_status[0x100];

#define ENTER_WORD_IN snprintf(yylval.word, sizeof(yylval.word) / sizeof(yylval.word[0]), "%s", yytext)

#ifdef ENABLE_LEX_TRACE
#define LOG_DEBUG(DATA) snprintf(lexer_status, sizeof(lexer_status) / sizeof(lexer_status[0]), "%s: \"%s\"\n", DATA, yytext)
#define LOG_DEBUG_EXT(DATA, TRANSLATE) snprintf(lexer_status, sizeof(lexer_status) / sizeof(lexer_status[0]), "%s: \"%s\" which is \"%s\"\n", DATA, yytext, TRANSLATE)
#else
#define LOG_DEBUG(DATA)
#define LOG_DEBUG_EXT(DATA, TRANSLATE)
#endif

int lex_line = 1;
int lex_word_count = 0;

int translateNumberWord(const char *word, char *prefix);

#include "correlatives.h"
#include "pronouns.h"

%}

%%
Sie { LOG_DEBUG("GROUP_BEGIN"); lex_word_count++; return GROUP_BEGIN; }
Soe { LOG_DEBUG("GROUP_END"); lex_word_count++; return GROUP_END; }
Tie { LOG_DEBUG("IF"); lex_word_count++; return IF; }
Teh { LOG_DEBUG("ELIF"); lex_word_count++; return ELIF; }
Tee { LOG_DEBUG("ELSE"); lex_word_count++; return ELSE; }
Toe { LOG_DEBUG("THEN"); lex_word_count++; return THEN; }
See { LOG_DEBUG("DELIMITER END"); lex_word_count++; ENTER_WORD_IN; return DELIMITER; }
Qee { LOG_DEBUG("DELIMITER END | Yes/No Question"); lex_word_count++; ENTER_WORD_IN; return DELIMITER; }
END { LOG_DEBUG("END"); lex_word_count++; ENTER_WORD_IN; return END; }
Seh { LOG_DEBUG("CONJUNCTION 'and"); lex_word_count++; ENTER_WORD_IN; return CONJUNCTION; }
Sel { LOG_DEBUG("CONJUNCTION 'or'"); lex_word_count++; ENTER_WORD_IN; return CONJUNCTION; }
Tel { LOG_DEBUG("CONJUNCTION 'because'"); lex_word_count++; ENTER_WORD_IN; return CONJUNCTION; }
Weh { LOG_DEBUG("CONJUNCTION 'so'"); lex_word_count++; ENTER_WORD_IN; return CONJUNCTION; }
Qel { LOG_DEBUG("PHRASE_CONJUNCTION 'and'"); lex_word_count++; ENTER_WORD_IN; return PHRASE_CONJUNCTION; }
Qeh { LOG_DEBUG("PHRASE_CONJUNCTION 'or'"); lex_word_count++; ENTER_WORD_IN; return PHRASE_CONJUNCTION; }
SelSel { LOG_DEBUG("PROPOSITION 'similar to'"); lex_word_count++; ENTER_WORD_IN; return PROPOSITION; }
TelSeh { LOG_DEBUG("PROPOSITION 'because'"); lex_word_count++; ENTER_WORD_IN; return PROPOSITION; }
TeeWel { LOG_DEBUG("PROPOSITION 'using'"); lex_word_count++; ENTER_WORD_IN; return PROPOSITION; }
QeeTelQee { LOG_DEBUG("PROPOSITION 'to'"); lex_word_count++; ENTER_WORD_IN; return PROPOSITION; }
TelQeeTel { LOG_DEBUG("PROPOSITION 'from'"); lex_word_count++; ENTER_WORD_IN; return PROPOSITION; }
QehTel { LOG_DEBUG("PROPOSITION 'on|at'"); lex_word_count++; ENTER_WORD_IN; return PROPOSITION; }
QelSee { LOG_DEBUG("PROPOSITION 'in/inside of'"); lex_word_count++; ENTER_WORD_IN; return PROPOSITION; }
WelQeeQee { LOG_DEBUG("PROPOSITION 'for'"); lex_word_count++; ENTER_WORD_IN; return PROPOSITION; }
SehWee { LOG_DEBUG("PROPOSITION 'because of'"); lex_word_count++; ENTER_WORD_IN; return PROPOSITION; }
SeeSee { LOG_DEBUG("PROPOSITION 'of'"); lex_word_count++; ENTER_WORD_IN; return PROPOSITION; }
TeeWelTee { LOG_DEBUG("NUMBER_SIGN"); lex_word_count++; yylval.number =  1; return NUMBER_SIGN; }
SeeWehTee { LOG_DEBUG("NUMBER_SIGN"); lex_word_count++; yylval.number = -1; return NUMBER_SIGN; }
(Wee)?(Tee)?(Qee)?(See)?Te[lh] {
    lex_word_count++;

    LOG_DEBUG("NUMBER_PLACE");
    // WARNING: If you change this regex, make sure that this code will still work. This code relies on the structure of this regex.

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
    lex_word_count++;

    LOG_DEBUG("NUMBER_100");
    // WARNING: If you change this regex, make sure that this code will still work. This code relies on the structure of this regex.

    char postfix = '@';
    yylval.number = translateNumberWord(yytext, &postfix);

    return NUMBER_100;
}
(Wel)?(Tel)?(Qel)?(Sel)?Tee {
    lex_word_count++;

    LOG_DEBUG("NUMBER_10");
    // WARNING: If you change this regex, make sure that this code will still work. This code relies on the structure of this regex.

    char postfix = '@';
    yylval.number = translateNumberWord(yytext, &postfix);

    return NUMBER_10;
}
(Wee)?(Tee)?(Qee)?(See)?Tee {
    lex_word_count++;

    LOG_DEBUG("NUMBER_1");
    // WARNING: If you change this regex, make sure that this code will still work. This code relies on the structure of this regex.

    char postfix = '@';
    yylval.number = translateNumberWord(yytext, &postfix);

    return NUMBER_1;
}
[SWTQ]ie([SWTQ]el)?[SW]oe(Toe)? {
    lex_word_count++;

    // WARNING: If you change this regex, make sure that this code will still work. This code relies on the structure of this regex.

    int person; // 4 possibilities
    int gender = 0; // 0 for unspecified. 5 possibilities.
    int type; // 2 possibilities
    int plurality = 0; // 0 for singlular, and 1 for plural.

    const char TABLE[] = "SWTQ";

    char *head = yytext;

    // Choose person. 1st person, 1st person exclusionary, 2nd person, 3rd person.
    for(int i = 0; TABLE[i] != '\0'; i++) {
        if(head[0] == TABLE[i])
            person = i;
    }

    head += 3; // Advance reading head.

    // If gender is specified.
    if(head[1] == 'e' && head[2] == 'l') {
        // Choose gender
        for(int i = 0; TABLE[i] != '\0'; i++) {
            if(head[0] == TABLE[i])
                gender = i;
        }

        gender++;

        head += 3; // Advance reading head.
    }

    // Choose either noun or possessive adjective
    for(int i = 0; TABLE[i] != '\0'; i++) {
        if(head[0] == TABLE[i])
            type = i;
    }

    head += 3; // Advance reading head.

    if(head[0] != '\0')
        plurality = 1;

    LOG_DEBUG_EXT("PRONOUN", PRONOUN_TABLE[person][gender][type][plurality]);

    snprintf(yylval.word, sizeof(yylval.word) / sizeof(yylval.word[0]), "%s", PRONOUN_TABLE[person][gender][type][plurality]);

    if(type == 0)
        return NOUN;
    else
        return ADJECTIVE;
}
((QieQelQoe)|(Se[hl])|([TW]eh))(See)((Qel)|(QolTeh)|(Tel)|(Weh)|(Woe)) {
    lex_word_count++;

    // WARNING: If you change this regex, make sure that this code will still work. This code relies on the structure of this regex.

    int type_index;
    int kind_index;
    int offset;

    switch(yytext[0]) {
        case 'Q':
            type_index = CORREL_NONE;
            offset = 4 * 3;
            break;
        case 'S':
            if(yytext[2] == 'h')
                type_index = CORREL_EVERY;
            else
                type_index = CORREL_SOME;
            offset = 2 * 3;
            break;
        case 'T':
            type_index = CORREL_WHAT;
            offset = 2 * 3;
            break;
        case 'W':
            type_index = CORREL_THAT;
            offset = 2 * 3;
            break;
    }

    switch(yytext[offset]) {
        case 'Q':
            if(yytext[offset + 1] == 'e')
                kind_index = CORREL_PLACE;
            else
                kind_index = CORREL_AMOUNT;
            break;
        case 'T':
            kind_index = CORREL_REASON;
            break;
        case 'W':
            if(yytext[offset + 1] == 'e')
                kind_index = CORREL_TIME;
            else
                kind_index = CORREL_DEMONSTRATIVE;
            break;
    }

    snprintf(yylval.word, sizeof(yylval.word) / sizeof(yylval.word[0]), "%s", CORRELATIVE_TABLE[kind_index][type_index]);

    LOG_DEBUG_EXT("CORRELATIVE", yylval.word);

    return NOUN;
}
([SQTW][ieo][ehl])+Sie(Toe)? { lex_word_count++; LOG_DEBUG("NOUN"); ENTER_WORD_IN; return NOUN; }
([SQTW][ieo][ehl])+Qie { lex_word_count++; LOG_DEBUG("ADJECTIVE"); ENTER_WORD_IN; return ADJECTIVE; }
([SQTW][ieo][ehl])+Qee[SW]e[ehl] {
    lex_word_count++;

    LOG_DEBUG("VERB");

    // Weh: Past
    // Wee: Present
    // Wel: Future
    // Seh: Conditional
    // See: Command
    // Sel: Infinitive

    ENTER_WORD_IN;

    return VERB;
}
([SQTW][ieo][ehl])+Wie { lex_word_count++; LOG_DEBUG("ADVERB"); ENTER_WORD_IN; return ADVERB; }
\n { lex_line++; }
[ \t]+ ; /* Do nothing */
[^[:space:]]+  { snprintf(lexer_status, sizeof(lexer_status) / sizeof(lexer_status[0]), "Lexer Error: \"%s\" is not a valid word in the language on line %d!\n", yytext, lex_line); }
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
