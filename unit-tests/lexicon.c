#include "grammer.h"

#include <stdio.h>

void lexerTestCallback(int token_type, const YYSTYPE *const yystype);

const char *const tokenTypeString(int token_type);

static const char text[] = "SieSoe SieSoeToe SieWoe SieWoeToe SieSelSoe SieSelSoeToe SieSelWoe SieSelWoeToe SieWelSoe SieWelSoeToe SieWelWoe SieWelWoeToe SieTelSoe SieTelSoeToe SieTelWoe SieTelWoeToe SieQelSoe SieQelSoeToe SieQelWoe SieQelWoeToe WieSoe WieSoeToe WieWoe WieWoeToe WieSelSoe WieSelSoeToe WieSelWoe WieSelWoeToe WieWelSoe WieWelSoeToe WieWelWoe WieWelWoeToe WieTelSoe WieTelSoeToe WieTelWoe WieTelWoeToe WieQelSoe WieQelSoeToe WieQelWoe WieQelWoeToe TieSoe TieSoeToe TieWoe TieWoeToe TieSelSoe TieSelSoeToe TieSelWoe TieSelWoeToe TieWelSoe TieWelSoeToe TieWelWoe TieWelWoeToe TieTelSoe TieTelSoeToe TieTelWoe TieTelWoeToe TieQelSoe TieQelSoeToe TieQelWoe TieQelWoeToe QieSoe QieSoeToe QieWoe QieWoeToe QieSelSoe QieSelSoeToe QieSelWoe QieSelWoeToe QieWelSoe QieWelSoeToe QieWelWoe QieWelWoeToe QieTelSoe QieTelSoeToe QieTelWoe QieTelWoeToe QieQelSoe QieQelSoeToe QieQelWoe QieQelWoeToe Seh Sel Tel Weh Qel Qeh Tie Teh Tee Toe Sie Soe See Qee SelSel TelSeh TeeWel QeeTelQee TelQeeTel QehTel QelSee WelQeeQee SehWee SeeSee SehTee QehTee QehSehTee TehTee TehSehTee TehQehTee TehQehSehTee WehTee WehSehTee WehQehTee WehQehSehTee WehTehTee WehTehSehTee WehTehQehTee WehTehQehSehTee SeeTee QeeTee QeeSeeTee TeeTee TeeSeeTee TeeQeeTee TeeQeeSeeTee WeeTee WeeSeeTee WeeQeeTee WeeQeeSeeTee WeeTeeTee WeeTeeSeeTee WeeTeeQeeTee WeeTeeQeeSeeTee SelTee QelTee QelSelTee TelTee TelSelTee TelQelTee TelQelSelTee WelTee WelSelTee WelQelTee WelQelSelTee WelTelTee WelTelSelTee WelTelQelTee WelTelQelSelTee SeeTeh QeeTeh QeeSeeTeh TeeTeh TeeSeeTeh TeeQeeTeh TeeQeeSeeTeh WeeTeh WeeSeeTeh WeeQeeTeh WeeQeeSeeTeh WeeTeeTeh WeeTeeSeeTeh WeeTeeQeeTeh WeeTeeQeeSeeTeh SeeTel QeeTel QeeSeeTel TeeTel TeeSeeTel TeeQeeTel TeeQeeSeeTel WeeTel WeeSeeTel WeeQeeTel WeeQeeSeeTel WeeTeeTel WeeTeeSeeTel WeeTeeQeeTel WeeTeeQeeSeeTel TeeWelTee SeeWehTee QieQelQoeSeeQel SehSeeQel SelSeeQel TehSeeQel WehSeeQel QieQelQoeSeeQolTeh SehSeeQolTeh SelSeeQolTeh TehSeeQolTeh WehSeeQolTeh QieQelQoeSeeTel SehSeeTel SelSeeTel TehSeeTel WehSeeTel QieQelQoeSeeWeh SehSeeWeh SelSeeWeh TehSeeWeh WehSeeWeh QieQelQoeSeeWoe SehSeeWoe SelSeeWoe TehSeeWoe WehSeeWoe ";
static const int token_types[] = {NOUN, NOUN, ADJECTIVE, ADJECTIVE, NOUN, NOUN, ADJECTIVE, ADJECTIVE, NOUN, NOUN, ADJECTIVE, ADJECTIVE, NOUN, NOUN, ADJECTIVE, ADJECTIVE, NOUN, NOUN, ADJECTIVE, ADJECTIVE, NOUN, NOUN, ADJECTIVE, ADJECTIVE, NOUN, NOUN, ADJECTIVE, ADJECTIVE, NOUN, NOUN, ADJECTIVE, ADJECTIVE, NOUN, NOUN, ADJECTIVE, ADJECTIVE, NOUN, NOUN, ADJECTIVE, ADJECTIVE, NOUN, NOUN, ADJECTIVE, ADJECTIVE, NOUN, NOUN, ADJECTIVE, ADJECTIVE, NOUN, NOUN, ADJECTIVE, ADJECTIVE, NOUN, NOUN, ADJECTIVE, ADJECTIVE, NOUN, NOUN, ADJECTIVE, ADJECTIVE, NOUN, NOUN, ADJECTIVE, ADJECTIVE, NOUN, NOUN, ADJECTIVE, ADJECTIVE, NOUN, NOUN, ADJECTIVE, ADJECTIVE, NOUN, NOUN, ADJECTIVE, ADJECTIVE, NOUN, NOUN, ADJECTIVE, ADJECTIVE, CONJUNCTION, CONJUNCTION, CONJUNCTION, CONJUNCTION, PHRASE_CONJUNCTION, PHRASE_CONJUNCTION, IF, ELIF, ELSE, THEN, GROUP_BEGIN, GROUP_END, DELIMITER, DELIMITER, PROPOSITION, PROPOSITION, PROPOSITION, PROPOSITION, PROPOSITION, PROPOSITION, PROPOSITION, PROPOSITION, PROPOSITION, PROPOSITION, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_SIGN, NUMBER_SIGN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, };
int token_type_index = 0;
int problem = 0;

int main() {
    lex_callback = lexerTestCallback;

    lexerParse(text);

    return problem;
}

extern char *yytext;

void lexerTestCallback(int token_type, const YYSTYPE *const yystype) {
    if(problem)
        return;

    switch(token_type) {
        case GROUP_BEGIN:
        case GROUP_END:
        case END:
        case IF:
        case ELIF:
        case ELSE:
        case THEN: {
            printf("%s(%d)\n", tokenTypeString(token_type), token_type);
            break;
        }
        case DELIMITER:
        case CONJUNCTION:
        case PHRASE_CONJUNCTION:
        case PROPOSITION:
        case NOUN:
        case ADJECTIVE:
        case VERB:
        case ADVERB:
        case NOT_A_KEYWORD:
        {
            printf("%s(%d) = \"%s\"\n", tokenTypeString(token_type), token_type, yystype->word);
            break;
        }
        case NUMBER_SIGN:
        case NUMBER_100:
        case NUMBER_10:
        case NUMBER_1:
        {
            printf("%s(%d) = %lli\n", tokenTypeString(token_type), token_type, yystype->number);
            break;
        }
        case NUMBER_PLACE:
        {
            printf("%s(%d) = %Lf\n", tokenTypeString(token_type), token_type, yystype->real_number);
            break;
        }
        default: {
            printf("%s(%d) = %s\n", tokenTypeString(token_type), token_type, yystype->word);
        }
    }

    if(token_types[token_type_index] != token_type) {
        printf("Type wrong expected %s(%d) for word at %i index. %s\n", tokenTypeString(token_types[token_type_index]), token_types[token_type_index], token_type_index, yytext);
        problem = 1;
    }
    token_type_index++;
}

static const char tokens[][19] = {
    "TOKEN_TYPE UNDER",
    "TOKEN_TYPE OVER",
    "GROUP_BEGIN",
    "GROUP_END",
    "END",
    "IF",
    "ELIF",
    "ELSE",
    "THEN",
    "DELIMITER",
    "CONJUNCTION",
    "PHRASE_CONJUNCTION",
    "PROPOSITION",
    "NOUN",
    "ADJECTIVE",
    "VERB",
    "ADVERB",
    "NOT_A_KEYWORD",
    "NUMBER_SIGN",
    "NUMBER_100",
    "NUMBER_10",
    "NUMBER_1",
    "NUMBER_PLACE"
};

const char *const tokenTypeString(int token_type) {

    if(token_type < 257)
        return tokens[0];

    if(token_type > 277)
        return tokens[1];

    return tokens[token_type - 257 + 2];
}
