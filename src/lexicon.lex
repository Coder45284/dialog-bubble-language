%{
#include <stdio.h>
#include "y.tab.h"

char lexer_status[0x100];

int lex_last_token = 0; // None
int lex_line = 1;
int lex_word_count = 0;

void lexerNullCallback(int token_type, const YYSTYPE *const yystype);

void (*lex_callback)(int token_type, const YYSTYPE *const yystype) = lexerNullCallback;

#define LEX_TOKEN_RETURN(x) {lex_word_count++; lex_last_token = x; lex_callback(x, &yylval); return x;}

#define ENTER_WORD_IN snprintf(yylval.word, sizeof(yylval.word) / sizeof(yylval.word[0]), "%s", yytext)
#define ENTER_WORD_TEXT(x) snprintf(yylval.word, sizeof(yylval.word) / sizeof(yylval.word[0]), "%s", x)

#ifdef ENABLE_LEX_TRACE
#define LOG_DEBUG(DATA) snprintf(lexer_status, sizeof(lexer_status) / sizeof(lexer_status[0]), "%s: \"%s\"\n", DATA, yytext)
#define LOG_DEBUG_EXT(DATA, TRANSLATE) snprintf(lexer_status, sizeof(lexer_status) / sizeof(lexer_status[0]), "%s: \"%s\" which is \"%s\"\n", DATA, yytext, TRANSLATE)
#else
#define LOG_DEBUG(DATA)
#define LOG_DEBUG_EXT(DATA, TRANSLATE)
#endif

int translateNumberWord(const char *word, char *prefix);

#include "correlatives.h"
#include "pronouns.h"

%}

%%
Sie { LOG_DEBUG("GROUP_BEGIN"); LEX_TOKEN_RETURN(GROUP_BEGIN); }
Soe { LOG_DEBUG("GROUP_END"); LEX_TOKEN_RETURN(GROUP_END); }
Tie { LOG_DEBUG("IF"); LEX_TOKEN_RETURN(IF); }
Teh { LOG_DEBUG("ELIF"); LEX_TOKEN_RETURN(ELIF); }
Tee { LOG_DEBUG("ELSE"); LEX_TOKEN_RETURN(ELSE); }
Toe { LOG_DEBUG("THEN"); LEX_TOKEN_RETURN(THEN); }
See { LOG_DEBUG("DELIMITER END"); ENTER_WORD_TEXT("END"); LEX_TOKEN_RETURN(DELIMITER); }
Qee { LOG_DEBUG("DELIMITER END | Yes/No Question"); ENTER_WORD_TEXT("END Yes|No"); LEX_TOKEN_RETURN(DELIMITER); }
END { LOG_DEBUG("END"); ENTER_WORD_IN; LEX_TOKEN_RETURN(END); }
Seh { LOG_DEBUG("CONJUNCTION 'and"); ENTER_WORD_TEXT("And"); LEX_TOKEN_RETURN(CONJUNCTION); }
Sel { LOG_DEBUG("CONJUNCTION 'or'"); ENTER_WORD_TEXT("Or"); LEX_TOKEN_RETURN(CONJUNCTION); }
Tel { LOG_DEBUG("CONJUNCTION 'because'"); ENTER_WORD_TEXT("Because"); LEX_TOKEN_RETURN(CONJUNCTION); }
Weh { LOG_DEBUG("CONJUNCTION 'so'"); ENTER_WORD_TEXT("So"); LEX_TOKEN_RETURN(CONJUNCTION); }
Qel { LOG_DEBUG("PHRASE_CONJUNCTION 'and'"); ENTER_WORD_TEXT("And"); LEX_TOKEN_RETURN(PHRASE_CONJUNCTION); }
Qeh { LOG_DEBUG("PHRASE_CONJUNCTION 'or'"); ENTER_WORD_TEXT("Or"); LEX_TOKEN_RETURN(PHRASE_CONJUNCTION); }
SelSel { LOG_DEBUG("PROPOSITION 'similar to'"); ENTER_WORD_TEXT("Similar to"); LEX_TOKEN_RETURN(PROPOSITION); }
TelSeh { LOG_DEBUG("PROPOSITION 'because'"); ENTER_WORD_TEXT("Because"); LEX_TOKEN_RETURN(PROPOSITION); }
TeeWel { LOG_DEBUG("PROPOSITION 'using'"); ENTER_WORD_TEXT("Using"); LEX_TOKEN_RETURN(PROPOSITION); }
QeeTelQee { LOG_DEBUG("PROPOSITION 'to'"); ENTER_WORD_TEXT("To"); LEX_TOKEN_RETURN(PROPOSITION); }
TelQeeTel { LOG_DEBUG("PROPOSITION 'from'"); ENTER_WORD_TEXT("From"); LEX_TOKEN_RETURN(PROPOSITION); }
QehTel { LOG_DEBUG("PROPOSITION 'on|at'"); ENTER_WORD_TEXT("On/At"); LEX_TOKEN_RETURN(PROPOSITION); }
QelSee { LOG_DEBUG("PROPOSITION 'in/inside of'"); ENTER_WORD_TEXT("In/Inside of"); LEX_TOKEN_RETURN(PROPOSITION); }
WelQeeQee { LOG_DEBUG("PROPOSITION 'for'"); ENTER_WORD_TEXT("For"); LEX_TOKEN_RETURN(PROPOSITION); }
SehWee { LOG_DEBUG("PROPOSITION 'because of'"); ENTER_WORD_TEXT("Because of"); LEX_TOKEN_RETURN(PROPOSITION); }
SeeSee { LOG_DEBUG("PROPOSITION 'of'"); ENTER_WORD_TEXT("Of"); LEX_TOKEN_RETURN(PROPOSITION); }
TeeWelTee { LOG_DEBUG("NUMBER_SIGN"); yylval.number =  1; LEX_TOKEN_RETURN(NUMBER_SIGN); }
SeeWehTee { LOG_DEBUG("NUMBER_SIGN"); yylval.number = -1; LEX_TOKEN_RETURN(NUMBER_SIGN); }
(Wee)?(Tee)?(Qee)?(See)?Te[lh] {
    LOG_DEBUG("NUMBER_PLACE");
    // WARNING: If you change this regex, make sure that this code will still work. This code relies on the structure of this regex.

    char postfix = '@';
    int number = translateNumberWord(yytext, &postfix);
    double real_number;

    static const long double BEFORE_DECIMAL[] = {
        2.441406e-04, 5.960464e-08, 1.455192e-11, 3.552714e-15, 8.673617e-19, 2.117582e-22, 5.169879e-26, 1.262177e-29, 3.081488e-33, 7.523164e-37, 1.836710e-40, 4.484155e-44, 1.094764e-47, 2.672765e-51, 6.525304e-55, 1.593092e-58};
    static const long double POST_DECIMAL[] = {
        1.000000e+00, 4.096000e+03, 1.677722e+07, 6.871948e+10, 2.814750e+14, 1.152922e+18, 4.722366e+21, 1.934281e+25, 7.922816e+28, 3.245186e+32, 1.329228e+36, 5.444518e+39, 2.230075e+43, 9.134385e+46, 3.741444e+50, 1.532496e+54};

    if(postfix == 'h')
        real_number = POST_DECIMAL[number - 1];
    else
        real_number = BEFORE_DECIMAL[number - 1];

    yylval.real_number = real_number;

    LEX_TOKEN_RETURN(NUMBER_PLACE);
}
(Weh)?(Teh)?(Qeh)?(Seh)?Tee {
    LOG_DEBUG("NUMBER_100");
    // WARNING: If you change this regex, make sure that this code will still work. This code relies on the structure of this regex.

    char postfix = '@';
    yylval.number = translateNumberWord(yytext, &postfix);

    LEX_TOKEN_RETURN(NUMBER_100);
}
(Wel)?(Tel)?(Qel)?(Sel)?Tee {
    LOG_DEBUG("NUMBER_10");
    // WARNING: If you change this regex, make sure that this code will still work. This code relies on the structure of this regex.

    char postfix = '@';
    yylval.number = translateNumberWord(yytext, &postfix);

    LEX_TOKEN_RETURN(NUMBER_10);
}
(Wee)?(Tee)?(Qee)?(See)?Tee {
    LOG_DEBUG("NUMBER_1");
    // WARNING: If you change this regex, make sure that this code will still work. This code relies on the structure of this regex.

    char postfix = '@';
    yylval.number = translateNumberWord(yytext, &postfix);

    LEX_TOKEN_RETURN(NUMBER_1);
}
[SWTQ]ie([SWTQ]el)?[SW]oe(Toe)? {
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
        LEX_TOKEN_RETURN(NOUN)
    else
        LEX_TOKEN_RETURN(ADJECTIVE)
}
((QieQelQoe)|(Se[hl])|([TW]eh))(See)((Qel)|(QolTeh)|(Tel)|(Weh)|(Woe)) {
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

    LEX_TOKEN_RETURN(NOUN);
}
([SQTW][ieo][ehl])+Sie(Toe)? { LOG_DEBUG("NOUN"); ENTER_WORD_IN; LEX_TOKEN_RETURN(NOUN); }
([SQTW][ieo][ehl])+Qie { LOG_DEBUG("ADJECTIVE"); ENTER_WORD_IN; LEX_TOKEN_RETURN(ADJECTIVE); }
([SQTW][ieo][ehl])+Qee[SW]e[ehl] {
    LOG_DEBUG("VERB");

    // Weh: Past
    // Wee: Present
    // Wel: Future
    // Seh: Conditional
    // See: Command
    // Sel: Infinitive

    ENTER_WORD_IN;

    LEX_TOKEN_RETURN(VERB);
}
([SQTW][ieo][ehl])+Wie { LOG_DEBUG("ADVERB"); ENTER_WORD_IN; LEX_TOKEN_RETURN(ADVERB); }
\n { lex_line++; }
[ \t]+ ; /* Do nothing */
[^[:space:]]+  { snprintf(lexer_status, sizeof(lexer_status) / sizeof(lexer_status[0]), "Lexer Error: \"%s\" is not a valid word in the language on line %d!\n", yytext, lex_line); LEX_TOKEN_RETURN(NOT_A_KEYWORD); }
%%

int yywrap() {
    return 1;
}

int lexerParse(const char *string) {
    lex_line = 1;
    lex_word_count = 0;

    YY_BUFFER_STATE buffer_state = yy_scan_string (string);

    int yy_result = yylex();

    while(yy_result != 0)
        yy_result = yylex();

    yy_delete_buffer(buffer_state);

    return yy_result;
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
            for(int i = 0; i < 2; i++) {
                word_head++;
                if(word_head[0] == '\0')
                    return -2;
            }
            word_head++;
            if(word_head[0] == '\0'){
                word_head -= 3;
                pre = 4;
            }
            else
                number += NUMBERS[pre];
        }
    }

    if(word_head[0] != 'T')
        return -3;
    word_head++;
    if(word_head[0] != 'e')
        return -4;
    word_head++;
    if(word_head[0] == '\0')
        return -5;

    *prefix = word_head[0];

    return number;
}

void lexerNullCallback(int token_type, const YYSTYPE *const yystype) {
    // No implementation
}
