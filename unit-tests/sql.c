#include "sql.h"

#include <stdio.h>

#define STATUS_CHECK(expected, expected_text, function, word)\
{\
    int status = function;\
    if(status != expected) {\
        problem |= 1;\
        printf("%s %s returned %d\n", expected_text, word, status);\
    }\
}

int main() {
    int problem = 0;

    STATUS_CHECK(SQL_NOT_INIT, "SQL_NOT_INIT", sqlGetWordIDLanguage("QeeSoeWee"), "sqlGetWordIDLanguage");
    STATUS_CHECK(SQL_NOT_INIT, "SQL_NOT_INIT", sqlGetWordIDEnglish("RUN"), "sqlGetWordIDEnglish");

    {
        WordDefinition word_def = {"QeeSoeWee", "VERB", "RUN", "The action of running."};

        STATUS_CHECK(SQL_NOT_INIT, "SQL_NOT_INIT", sqlAddWord(&word_def), "sqlAddWord");
        STATUS_CHECK(SQL_NOT_INIT, "SQL_NOT_INIT", sqlUpdateWord(1, &word_def), "sqlUpdateWord");
        STATUS_CHECK(SQL_NOT_INIT, "SQL_NOT_INIT", sqlGetWord(1, &word_def), "sqlGetWord");
        STATUS_CHECK(SQL_NOT_INIT, "SQL_NOT_INIT", sqlRemoveWord(1), "sqlRemoveWord");
    }

    // Do not need to write to disk.
    sqlInit(":memory:");

    // TODO Add checks if words are successfully created.
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

    // Check words.

    {
        WordDefinition word_def = {"Weh", "PARTITION", "CARD", "It is non sense."};
        sqlUpdateWord(sqlGetWordIDEnglish("CART"), &word_def);
    }

    sqlDeinit();

    return problem;
}
