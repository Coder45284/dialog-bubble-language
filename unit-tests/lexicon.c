#include "grammer.h"

#include <stdio.h>
#include <string.h>

void lexerTestCallback(int token_type, const YYSTYPE *const yystype);

const char *const tokenTypeString(int token_type);

typedef struct {
    int         token_type;
    const char *string;
} ExpectedResult;

typedef struct {
    char           *text;
    ExpectedResult results[];
} Test;

static const Test main_test = {
    "SieSoe SieSoeToe SieWoe SieWoeToe SieSelSoe SieSelSoeToe SieSelWoe SieSelWoeToe SieWelSoe SieWelSoeToe SieWelWoe SieWelWoeToe SieTelSoe SieTelSoeToe SieTelWoe SieTelWoeToe SieQelSoe SieQelSoeToe SieQelWoe SieQelWoeToe WieSoe WieSoeToe WieWoe WieWoeToe WieSelSoe WieSelSoeToe WieSelWoe WieSelWoeToe WieWelSoe WieWelSoeToe WieWelWoe WieWelWoeToe WieTelSoe WieTelSoeToe WieTelWoe WieTelWoeToe WieQelSoe WieQelSoeToe WieQelWoe WieQelWoeToe TieSoe TieSoeToe TieWoe TieWoeToe TieSelSoe TieSelSoeToe TieSelWoe TieSelWoeToe TieWelSoe TieWelSoeToe TieWelWoe TieWelWoeToe TieTelSoe TieTelSoeToe TieTelWoe TieTelWoeToe TieQelSoe TieQelSoeToe TieQelWoe TieQelWoeToe QieSoe QieSoeToe QieWoe QieWoeToe QieSelSoe QieSelSoeToe QieSelWoe QieSelWoeToe QieWelSoe QieWelSoeToe QieWelWoe QieWelWoeToe QieTelSoe QieTelSoeToe QieTelWoe QieTelWoeToe QieQelSoe QieQelSoeToe QieQelWoe QieQelWoeToe Seh Sel Tel Weh Qel Qeh Tie Teh Tee Toe Sie Soe See Qee SelSel TelSeh TeeWel QeeTelQee TelQeeTel QehTel QelSee WelQeeQee SehWee SeeSee SehTee QehTee QehSehTee TehTee TehSehTee TehQehTee TehQehSehTee WehTee WehSehTee WehQehTee WehQehSehTee WehTehTee WehTehSehTee WehTehQehTee WehTehQehSehTee SelTee QelTee QelSelTee TelTee TelSelTee TelQelTee TelQelSelTee WelTee WelSelTee WelQelTee WelQelSelTee WelTelTee WelTelSelTee WelTelQelTee WelTelQelSelTee SeeTee QeeTee QeeSeeTee TeeTee TeeSeeTee TeeQeeTee TeeQeeSeeTee WeeTee WeeSeeTee WeeQeeTee WeeQeeSeeTee WeeTeeTee WeeTeeSeeTee WeeTeeQeeTee WeeTeeQeeSeeTee SeeTel QeeTel QeeSeeTel TeeTel TeeSeeTel TeeQeeTel TeeQeeSeeTel WeeTel WeeSeeTel WeeQeeTel WeeQeeSeeTel WeeTeeTel WeeTeeSeeTel WeeTeeQeeTel WeeTeeQeeSeeTel SeeTeh QeeTeh QeeSeeTeh TeeTeh TeeSeeTeh TeeQeeTeh TeeQeeSeeTeh WeeTeh WeeSeeTeh WeeQeeTeh WeeQeeSeeTeh WeeTeeTeh WeeTeeSeeTeh WeeTeeQeeTeh WeeTeeQeeSeeTeh TeeWelTee SeeWehTee QieQelQoeSeeQel SehSeeQel SelSeeQel TehSeeQel WehSeeQel QieQelQoeSeeQolTeh SehSeeQolTeh SelSeeQolTeh TehSeeQolTeh WehSeeQolTeh QieQelQoeSeeTel SehSeeTel SelSeeTel TehSeeTel WehSeeTel QieQelQoeSeeWeh SehSeeWeh SelSeeWeh TehSeeWeh WehSeeWeh QieQelQoeSeeWoe SehSeeWoe SelSeeWoe TehSeeWoe WehSeeWoe",
    {
        {NOUN, "I/Me"}, {NOUN, "We/Us"}, {ADJECTIVE, "My"}, {ADJECTIVE, "Our"},
        {NOUN, "I/Me (female)"}, {NOUN, "We/Us (female)"}, {ADJECTIVE, "My (female)"}, {ADJECTIVE, "Our (female)"},
        {NOUN, "I/Me (neuter)"}, {NOUN, "We/Us (neuter)"}, {ADJECTIVE, "My (neuter)"}, {ADJECTIVE, "Our (neuter)"},
        {NOUN, "I/Me (male)"}, {NOUN, "We/Us (male)"}, {ADJECTIVE, "My (male)"}, {ADJECTIVE, "Our (male)"},
        {NOUN, "I/Me (non-binary)"}, {NOUN, "We/Us (non-binary)"}, {ADJECTIVE, "My (non-binary)"}, {ADJECTIVE, "Our (non-binary)"},
        {NOUN, "I/Me, not you"}, {NOUN, "We/Us, not you"}, {ADJECTIVE, "My, not you"}, {ADJECTIVE, "Our, not you"},
        {NOUN, "I/Me (female), not you"}, {NOUN, "We/Us (female), not you"}, {ADJECTIVE, "My (female), not you"}, {ADJECTIVE, "Our (female), not you"},
        {NOUN, "I/Me (neuter), not you"}, {NOUN, "We/Us (neuter), not you"}, {ADJECTIVE, "My (neuter), not you"}, {ADJECTIVE, "Our (neuter), not you"},
        {NOUN, "I/Me (male), not you"}, {NOUN, "We/Us (male), not you"}, {ADJECTIVE, "My (male), not you"}, {ADJECTIVE, "Our (male), not you"},
        {NOUN, "I/Me (non-binary), not you"}, {NOUN, "We/Us (non-binary), not you"}, {ADJECTIVE, "My (non-binary), not you"}, {ADJECTIVE, "Our (non-binary), not you"},
        {NOUN, "They (singular)"}, {NOUN, "They"}, {ADJECTIVE, "Their (singular)"}, {ADJECTIVE, "Their"},
        {NOUN, "She"}, {NOUN, "They (all-female)"}, {ADJECTIVE, "Her"}, {ADJECTIVE, "Their (all-female)"},
        {NOUN, "It"}, {NOUN, "They (all-neuter)"}, {ADJECTIVE, "Its"}, {ADJECTIVE, "Their (all-neuter)"},
        {NOUN, "He"}, {NOUN, "They (all-male)"}, {ADJECTIVE, "His"}, {ADJECTIVE, "Their (all-male)"},
        {NOUN, "They (all-non-binary,singular)"}, {NOUN, "They (all-non-binary,plural)"}, {ADJECTIVE, "Their (all-non-binary,singular)"}, {ADJECTIVE, "Their (all-non-binary,plural)"},
        {NOUN, "You (singular)"}, {NOUN, "You all (plural)"}, {ADJECTIVE, "Your (singular)"}, {ADJECTIVE, "You all's (plural)"},
        {NOUN, "You (female,singular)"}, {NOUN, "You all (female,plural)"}, {ADJECTIVE, "Your (female,singular)"}, {ADJECTIVE, "You all's (female,plural)"},
        {NOUN, "You (neuter,singular)"}, {NOUN, "You all (neuter,plural)"}, {ADJECTIVE, "Your (neuter,singular)"}, {ADJECTIVE, "You all's (neuter,plural)"},
        {NOUN, "You (male,singular)"}, {NOUN, "You all (male,plural)"}, {ADJECTIVE, "Your (male,singular)"}, {ADJECTIVE, "You all's (male,plural)"},
        {NOUN, "You (non-binary,singular)"}, {NOUN, "You all (non-binary,plural)"}, {ADJECTIVE, "Your (non-binary,singular)"}, {ADJECTIVE, "You all's (non-binary,plural)"},
        {CONJUNCTION, "And"}, {CONJUNCTION, "Or"}, {CONJUNCTION, "Because"}, {CONJUNCTION, "So"},
        {PHRASE_CONJUNCTION, "And"}, {PHRASE_CONJUNCTION, "Or"},
        {IF, ""}, {ELIF, ""}, {ELSE, ""}, {THEN, ""}, {GROUP_BEGIN, ""}, {GROUP_END, ""},
        {DELIMITER, "END"}, {DELIMITER, "END Yes|No"},
        {PROPOSITION, "Similar to"}, {PROPOSITION, "Because"}, {PROPOSITION, "Using"}, {PROPOSITION, "To"}, {PROPOSITION, "From"}, {PROPOSITION, "On/At"}, {PROPOSITION, "In/Inside of"}, {PROPOSITION, "For"}, {PROPOSITION, "Because of"}, {PROPOSITION, "Of"},
        {NUMBER_100, "1"}, {NUMBER_100, "2"}, {NUMBER_100, "3"}, {NUMBER_100, "4"}, {NUMBER_100, "5"}, {NUMBER_100, "6"}, {NUMBER_100, "7"}, {NUMBER_100, "8"}, {NUMBER_100, "9"}, {NUMBER_100, "10"}, {NUMBER_100, "11"}, {NUMBER_100, "12"}, {NUMBER_100, "13"}, {NUMBER_100, "14"}, {NUMBER_100, "15"},
        {NUMBER_10, "1"}, {NUMBER_10, "2"}, {NUMBER_10, "3"}, {NUMBER_10, "4"}, {NUMBER_10, "5"}, {NUMBER_10, "6"}, {NUMBER_10, "7"}, {NUMBER_10, "8"}, {NUMBER_10, "9"}, {NUMBER_10, "10"}, {NUMBER_10, "11"}, {NUMBER_10, "12"}, {NUMBER_10, "13"}, {NUMBER_10, "14"}, {NUMBER_10, "15"},
        {NUMBER_1, "1"}, {NUMBER_1, "2"}, {NUMBER_1, "3"}, {NUMBER_1, "4"}, {NUMBER_1, "5"}, {NUMBER_1, "6"}, {NUMBER_1, "7"}, {NUMBER_1, "8"}, {NUMBER_1, "9"}, {NUMBER_1, "10"}, {NUMBER_1, "11"}, {NUMBER_1, "12"}, {NUMBER_1, "13"}, {NUMBER_1, "14"}, {NUMBER_1, "15"},
        {NUMBER_PLACE, "2.441406e-04"}, {NUMBER_PLACE, "5.960464e-08"}, {NUMBER_PLACE, "1.455192e-11"}, {NUMBER_PLACE, "3.552714e-15"}, {NUMBER_PLACE, "8.673617e-19"}, {NUMBER_PLACE, "2.117582e-22"}, {NUMBER_PLACE, "5.169879e-26"}, {NUMBER_PLACE, "1.262177e-29"}, {NUMBER_PLACE, "3.081488e-33"}, {NUMBER_PLACE, "7.523164e-37"}, {NUMBER_PLACE, "1.836710e-40"}, {NUMBER_PLACE, "4.484155e-44"}, {NUMBER_PLACE, "1.094764e-47"}, {NUMBER_PLACE, "2.672765e-51"}, {NUMBER_PLACE, "6.525304e-55"},
        {NUMBER_PLACE, "1.000000e+00"}, {NUMBER_PLACE, "4.096000e+03"}, {NUMBER_PLACE, "1.677722e+07"}, {NUMBER_PLACE, "6.871948e+10"}, {NUMBER_PLACE, "2.814750e+14"}, {NUMBER_PLACE, "1.152922e+18"}, {NUMBER_PLACE, "4.722366e+21"}, {NUMBER_PLACE, "1.934281e+25"}, {NUMBER_PLACE, "7.922816e+28"}, {NUMBER_PLACE, "3.245186e+32"}, {NUMBER_PLACE, "1.329228e+36"}, {NUMBER_PLACE, "5.444518e+39"}, {NUMBER_PLACE, "2.230075e+43"}, {NUMBER_PLACE, "9.134385e+46"}, {NUMBER_PLACE, "3.741444e+50"},
        {NUMBER_SIGN, "1"}, {NUMBER_SIGN, "-1"},
        {NOUN, "Nowhere"}, {NOUN, "Everywhere"}, {NOUN, "Somewhere"}, {NOUN, "Where"}, {NOUN, "There"},
        {NOUN, "None (no amount)"}, {NOUN, "All (amount)"}, {NOUN, "Some (amount)"}, {NOUN, "How much"}, {NOUN, "That amount"},
        {NOUN, "No reason"}, {NOUN, "Every reason"}, {NOUN, "Some reason"}, {NOUN, "Why"}, {NOUN, "That reason"},
        {NOUN, "Never"}, {NOUN, "Everytime"}, {NOUN, "Sometime"}, {NOUN, "When"}, {NOUN, "Then"},
        {NOUN, "No one/Nothing"}, {NOUN, "Everyone/Everything"}, {NOUN, "Someone/Something"}, {NOUN, "Who/What"}, {NOUN, "That/That (one)"}
    }
};

int token_type_index = 0;
int problem = 0;

int main() {
    lex_callback = lexerTestCallback;

    lexerParse(main_test.text);

    return problem;
}

extern char *yytext;

void lexerTestCallback(int token_type, const YYSTYPE *const yystype) {
    char result[sizeof(YYSTYPE)];

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
            result[0] = '\0';
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

            snprintf(result, sizeof(result) / sizeof(result[0]), "%s", yystype->word);

            break;
        }
        case NUMBER_SIGN:
        case NUMBER_100:
        case NUMBER_10:
        case NUMBER_1:
        {
            printf("%s(%d) = %lli\n", tokenTypeString(token_type), token_type, yystype->number);

            snprintf(result, sizeof(result) / sizeof(result[0]), "%lli", yystype->number);

            break;
        }
        case NUMBER_PLACE:
        {
            printf("%s(%d) = %Le\n", tokenTypeString(token_type), token_type, yystype->real_number);

            snprintf(result, sizeof(result) / sizeof(result[0]), "%Le", yystype->real_number);

            break;
        }
        default: {
            printf("%s(%d) = %s\n", tokenTypeString(token_type), token_type, yystype->word);

            snprintf(result, sizeof(result) / sizeof(result[0]), "%s", "N/A");
        }
    }

    if(main_test.results[token_type_index].token_type != token_type) {
        printf("Type wrong expected %s(%d) for word at %i index. %s\n", tokenTypeString(main_test.results[token_type_index].token_type), main_test.results[token_type_index].token_type, token_type_index, yytext);
        problem = 1;
    }
    if(strcmp(main_test.results[token_type_index].string, result) != 0) {
        printf("Return does not match expected %s(%d) for word at %i index. Original Word: %s. Expected: \"%s\". Returned: \"%s\"\n", tokenTypeString(main_test.results[token_type_index].token_type), main_test.results[token_type_index].token_type, token_type_index, yytext, main_test.results[token_type_index].string, result);
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
