#include "grammer.h"

#include <stdio.h>
#include <string.h>

void lexerTestCallback(int token_type, const YYSTYPE *const yystype);

const char *const tokenTypeString(int token_type);

static const char text[] = "SieSoe SieSoeToe SieWoe SieWoeToe SieSelSoe SieSelSoeToe SieSelWoe SieSelWoeToe SieWelSoe SieWelSoeToe SieWelWoe SieWelWoeToe SieTelSoe SieTelSoeToe SieTelWoe SieTelWoeToe SieQelSoe SieQelSoeToe SieQelWoe SieQelWoeToe WieSoe WieSoeToe WieWoe WieWoeToe WieSelSoe WieSelSoeToe WieSelWoe WieSelWoeToe WieWelSoe WieWelSoeToe WieWelWoe WieWelWoeToe WieTelSoe WieTelSoeToe WieTelWoe WieTelWoeToe WieQelSoe WieQelSoeToe WieQelWoe WieQelWoeToe TieSoe TieSoeToe TieWoe TieWoeToe TieSelSoe TieSelSoeToe TieSelWoe TieSelWoeToe TieWelSoe TieWelSoeToe TieWelWoe TieWelWoeToe TieTelSoe TieTelSoeToe TieTelWoe TieTelWoeToe TieQelSoe TieQelSoeToe TieQelWoe TieQelWoeToe QieSoe QieSoeToe QieWoe QieWoeToe QieSelSoe QieSelSoeToe QieSelWoe QieSelWoeToe QieWelSoe QieWelSoeToe QieWelWoe QieWelWoeToe QieTelSoe QieTelSoeToe QieTelWoe QieTelWoeToe QieQelSoe QieQelSoeToe QieQelWoe QieQelWoeToe Seh Sel Tel Weh Qel Qeh Tie Teh Tee Toe Sie Soe See Qee SelSel TelSeh TeeWel QeeTelQee TelQeeTel QehTel QelSee WelQeeQee SehWee SeeSee SehTee QehTee QehSehTee TehTee TehSehTee TehQehTee TehQehSehTee WehTee WehSehTee WehQehTee WehQehSehTee WehTehTee WehTehSehTee WehTehQehTee WehTehQehSehTee SelTee QelTee QelSelTee TelTee TelSelTee TelQelTee TelQelSelTee WelTee WelSelTee WelQelTee WelQelSelTee WelTelTee WelTelSelTee WelTelQelTee WelTelQelSelTee SeeTee QeeTee QeeSeeTee TeeTee TeeSeeTee TeeQeeTee TeeQeeSeeTee WeeTee WeeSeeTee WeeQeeTee WeeQeeSeeTee WeeTeeTee WeeTeeSeeTee WeeTeeQeeTee WeeTeeQeeSeeTee SeeTel QeeTel QeeSeeTel TeeTel TeeSeeTel TeeQeeTel TeeQeeSeeTel WeeTel WeeSeeTel WeeQeeTel WeeQeeSeeTel WeeTeeTel WeeTeeSeeTel WeeTeeQeeTel WeeTeeQeeSeeTel SeeTeh QeeTeh QeeSeeTeh TeeTeh TeeSeeTeh TeeQeeTeh TeeQeeSeeTeh WeeTeh WeeSeeTeh WeeQeeTeh WeeQeeSeeTeh WeeTeeTeh WeeTeeSeeTeh WeeTeeQeeTeh WeeTeeQeeSeeTeh TeeWelTee SeeWehTee QieQelQoeSeeQel SehSeeQel SelSeeQel TehSeeQel WehSeeQel QieQelQoeSeeQolTeh SehSeeQolTeh SelSeeQolTeh TehSeeQolTeh WehSeeQolTeh QieQelQoeSeeTel SehSeeTel SelSeeTel TehSeeTel WehSeeTel QieQelQoeSeeWeh SehSeeWeh SelSeeWeh TehSeeWeh WehSeeWeh QieQelQoeSeeWoe SehSeeWoe SelSeeWoe TehSeeWoe WehSeeWoe";

static const int token_types[] = {
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    NOUN, NOUN, ADJECTIVE, ADJECTIVE,
    CONJUNCTION, CONJUNCTION, CONJUNCTION, CONJUNCTION,
    PHRASE_CONJUNCTION, PHRASE_CONJUNCTION,
    IF, ELIF, ELSE, THEN, GROUP_BEGIN, GROUP_END,
    DELIMITER, DELIMITER,
    PROPOSITION, PROPOSITION, PROPOSITION, PROPOSITION, PROPOSITION, PROPOSITION, PROPOSITION, PROPOSITION, PROPOSITION, PROPOSITION,
    NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100, NUMBER_100,
    NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10, NUMBER_10,
    NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1, NUMBER_1,
    NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE,
    NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE, NUMBER_PLACE,
    NUMBER_SIGN, NUMBER_SIGN,
    NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN, NOUN};

static const char *return_text[] = {
    "I/Me", "We/Us", "My", "Our",
    "I/Me (female)", "We/Us (female)", "My (female)", "Our (female)",
    "I/Me (neuter)", "We/Us (neuter)", "My (neuter)", "Our (neuter)",
    "I/Me (male)", "We/Us (male)", "My (male)", "Our (male)",
    "I/Me (non-binary)", "We/Us (non-binary)", "My (non-binary)", "Our (non-binary)",
    "I/Me, not you", "We/Us, not you", "My, not you", "Our, not you",
    "I/Me (female), not you", "We/Us (female), not you", "My (female), not you", "Our (female), not you",
    "I/Me (neuter), not you", "We/Us (neuter), not you", "My (neuter), not you", "Our (neuter), not you",
    "I/Me (male), not you", "We/Us (male), not you", "My (male), not you", "Our (male), not you",
    "I/Me (non-binary), not you", "We/Us (non-binary), not you", "My (non-binary), not you", "Our (non-binary), not you",
    "They (singular)", "They", "Their (singular)", "Their",
    "She", "They (all-female)", "Her", "Their (all-female)",
    "It", "They (all-neuter)", "Its", "Their (all-neuter)",
    "He", "They (all-male)", "His", "Their (all-male)",
    "They (all-non-binary,singular)", "They (all-non-binary,plural)", "Their (all-non-binary,singular)", "Their (all-non-binary,plural)",
    "You (singular)", "You all (plural)", "Your (singular)", "You all's (plural)",
    "You (female,singular)", "You all (female,plural)", "Your (female,singular)", "You all's (female,plural)",
    "You (neuter,singular)", "You all (neuter,plural)", "Your (neuter,singular)", "You all's (neuter,plural)",
    "You (male,singular)", "You all (male,plural)", "Your (male,singular)", "You all's (male,plural)",
    "You (non-binary,singular)", "You all (non-binary,plural)", "Your (non-binary,singular)", "You all's (non-binary,plural)",
    "And", "Or", "Because", "So", "And", "Or", "", "", "", "", "", "", "END", "END Yes|No", "Similar to", "Because", "Using", "To", "From", "On/At", "In/Inside of", "For", "Because of", "Of",
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15",
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15",
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15",
    "2.441406e-04", "5.960464e-08", "1.455192e-11", "3.552714e-15", "8.673617e-19", "2.117582e-22", "5.169879e-26", "1.262177e-29", "3.081488e-33", "7.523164e-37", "1.836710e-40", "4.484155e-44", "1.094764e-47", "2.672765e-51", "6.525304e-55",
    "1.000000e+00", "4.096000e+03", "1.677722e+07", "6.871948e+10", "2.814750e+14", "1.152922e+18", "4.722366e+21", "1.934281e+25", "7.922816e+28", "3.245186e+32", "1.329228e+36", "5.444518e+39", "2.230075e+43", "9.134385e+46", "3.741444e+50",
    "1", "-1",
    "Nowhere", "Everywhere", "Somewhere", "Where", "There", "None (no amount)", "All (amount)", "Some (amount)", "How much", "That amount", "No reason", "Every reason", "Some reason", "Why", "That reason", "Never", "Everytime", "Sometime", "When", "Then", "No one/Nothing", "Everyone/Everything", "Someone/Something", "Who/What", "That/That (one)"};

typedef struct {
    int         token_type;
    const char *string;
} ExpectedResult;

static const ExpectedResult expected_results[] = {
    {NOUN, "I/Me"}, {NOUN, "We/Us"}, {ADJECTIVE, "My"}, {ADJECTIVE, "Our"}
};

int token_type_index = 0;
int problem = 0;

int main() {
    printf("static const ExpectedResult expected_results[] = {\n    ");
    for(int i = 0; i < sizeof(token_types)/sizeof(token_types[0]); i++) {
        printf("{%s, \"%s\"}, ", tokenTypeString(token_types[i]), return_text[i]);
    }
    printf("}\n");

    lex_callback = lexerTestCallback;

    lexerParse(text);

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

    if(token_types[token_type_index] != token_type) {
        printf("Type wrong expected %s(%d) for word at %i index. %s\n", tokenTypeString(token_types[token_type_index]), token_types[token_type_index], token_type_index, yytext);
        problem = 1;
    }
    if(strcmp(return_text[token_type_index], result) != 0) {
        printf("Return does not match expected %s(%d) for word at %i index. Original Word: %s. Expected: \"%s\". Returned: \"%s\"\n", tokenTypeString(token_types[token_type_index]), token_types[token_type_index], token_type_index, yytext, return_text[token_type_index], result);
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
