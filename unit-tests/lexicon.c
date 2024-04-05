#include "grammer.h"

#include <stdio.h>

void lexerTestCallback(int token_type, const YYSTYPE *const yystype);

const char *const tokenTypeString(int token_type);

int main() {
    static const char text[] = "Tie Sie TieSoeToe QeeQeeQie QeeTeeQie QelWelQie TeeWelTee SeeTeh SeeSee SeeQehSie WoeTehQie TeeWeeQie TeeWeeWeeQie SeeWehTee QeeSeeTeh WehSehTee TelQelTee WeeQeeTee QeeTeh WehTee SelTee WeeTeeTee SeeTeh QehSehTee TelSelTee WeeSeeTee Qel QieWelSoe TehQeeWee WeeQeeSie WoeTehQie QeeTeeQie SeeWehTee SeeTeh TelQelSelTee WeeTeeQeeSeeTee SeeSee QelQehSie TeeWeeWeeQie WoeTehQie QeeQeeQie TeeWelTee SeeTeh WehTehTee QeeTee SeeTel WehTehQehSehTee WelTelQelSelTee WeeTeeQeeSeeTee Seh SieSoe TehQeeWee SeeSeeSehSie Soe Toe Sie QelQehSie SeeSeeSeeQeeWel QelQehSie See Tie Sie SieSoe TehQeeWee TeeQeeSieToe Soe Toe Sie SieSoe TehQeeWee QeeToeWie TeeTeeSieToe See Soe Soe TeeWeeSie TehQeeWee QeeTeeTeeSie SeeSee QehQehTieSieToe SeeWehTee SeeTeh TeeSeeTee Qee WehSeeQel TehQeeWee TieWelSoe Qel QieWelSoe Tel WehSeeQolTeh TehQeeWee TieWelSoe See WehSeeTel TehQeeWee TieWelSoe See WehSeeWeh TehQeeWee TieWelSoe Sel WehSeeWoe TehQeeWee QeeTelQee TieWelSoe See";

    int status = 1;

    lex_callback = lexerTestCallback;

    lexerParse(text);

    return status;
}

void lexerTestCallback(int token_type, const YYSTYPE *const yystype) {
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
