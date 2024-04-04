#include "grammer.h"

#include <stdio.h>

void lexerTestCallback(int token_type, const YYSTYPE *const yystype);

const char *const tokenTypeString(int token_type);

int main() {
    static const char text[] = "Tie Sie TieSoeToe QeeQeeQie QeeTeeQie QelWelQie TeeWelTee SeeTeh SeeSee SeeQehSie WoeTehQie TeeWeeQie TeeWeeWeeQie SeeWehTee QeeSeeTeh WehSehTee TelQelTee WeeQeeTee QeeTeh WehTee SelTee WeeTeeTee SeeTeh QehSehTee TelSelTee WeeSeeTee Qel QieWelSoe TehQeeWee WeeQeeSie WoeTehQie QeeTeeQie SeeWehTee SeeTeh TelQelSelTee WeeTeeQeeSeeTee SeeSee QelQehSie TeeWeeWeeQie WoeTehQie QeeQeeQie TeeWelTee SeeTeh WehTehTee QeeTee SeeTel WehTehQehSehTee WelTelQelSelTee WeeTeeQeeSeeTee Seh SieSoe TehQeeWee SeeSeeSehSie See Soe Toe Sie QelQehSie SeeSeeSeeQeeWel QelQehSie See Tie Sie SieSoe TehQeeWee TeeQeeSieToe See Soe Toe Sie SieSoe TehQeeWee QeeToeWie TeeTeeSieToe See Soe Soe TeeWeeSie TehQeeWee QeeTeeTeeSie SeeSee QehQehTieSieToe SeeWehTee SeeTeh TeeSeeTee Qee WehSeeQel TehQeeWee TieWelSoe Qel QieWelSoe Tel WehSeeQolTeh TehQeeWee TieWelSoe See WehSeeTel TehQeeWee TieWelSoe See WehSeeWeh TehQeeWee TieWelSoe Sel WehSeeWoe TehQeeWee QeeTelQee TieWelSoe See";

    int status = 1;

    lex_callback = lexerTestCallback;

    lexerParse(text);

    return status;
}

void lexerTestCallback(int token_type, const YYSTYPE *const yystype) {
    printf("%s(%d) = %s\n", tokenTypeString(token_type), token_type, yystype->word);
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
