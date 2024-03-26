#include "sql.h"

#include <stdio.h>

#define STATUS_CHECK(function, word)\
{\
    int status = function;\
    if(status != SQL_NOT_INIT) {\
        problem |= 1;\
        printf("SQL_NOT_INIT %s returned %d\n", word, status);\
    }\
}

int main() {
    int problem = 0;

    STATUS_CHECK(sqlGetWordIDLanguage("QeeSoeWee"), "sqlGetWordIDLanguage");
    STATUS_CHECK(sqlGetWordIDEnglish("RUN"), "sqlGetWordIDEnglish");

    {
        WordDefinition word_def = {"QeeSoeWee", "VERB", "RUN", "The action of running."};

        STATUS_CHECK(sqlAddWord(&word_def), "sqlAddWord");
        STATUS_CHECK(sqlUpdateWord(1, &word_def), "sqlUpdateWord");
        STATUS_CHECK(sqlGetWord(1, &word_def), "sqlGetWord");
        STATUS_CHECK(sqlRemoveWord(1), "sqlRemoveWord");
    }

    sqlInit(":memory:");

    {
        WordDefinition word_def = {"QeeSoeWee", "VERB", "RUN", "The action of running."};
        sqlAddWord(&word_def);
    }
    {
        WordDefinition word_def = {"QeeSeeWel", "NOUN;ADJECTIVE", "RED", "The color red."};
        sqlAddWord(&word_def);
    }
    {
        WordDefinition word_def = {"QoeQoeWeh", "NOUN", "CART", "A cart like a shopping cart."};
        sqlAddWord(&word_def);
    }
    {
        WordDefinition word_def = {"ToeQeeWee", "ADVERB;ADJECTIVE", "SLOW", "Slow movement or to describe something with slow movement."};
        sqlAddWord(&word_def);
    }
    {
        WordDefinition word_def = {"Weh", "PARTITION", "CARD", "It is non sense."};
        sqlUpdateWord(sqlGetWordIDEnglish("CART"), &word_def);
    }

    sqlDeinit();

    return problem;
}
