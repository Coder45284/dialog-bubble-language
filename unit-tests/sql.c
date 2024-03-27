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

    WordDefinition word_defs[5] = {
        {"QeeSoeWee", "VERB", "RUN", "The action of running."},
        {"QeeSeeWel", "NOUN;ADJECTIVE", "RED", "The color red."},
        {"QoeQoeWeh", "NOUN", "CART", "A cart like a shopping cart."},
        {"ToeQeeWee", "ADVERB;ADJECTIVE", "SLOW", "Slow movement or to describe something with slow movement."},
        {"@", "@", "@", "@"}
    };

    int expected_returns[5] = {1, 2, 3, 4, SQL_DNE};

    for(int i = 0; i < 4; i++) {
        const char word[2] = {'1' + i, '\0'};

        STATUS_CHECK(1 + i, word, sqlAddWord(&word_defs[i]), "sqlAddWord");
    }

    char output[0x100];

    // Sanity checks
    for(int i = 0; i < 5; i++) {
        int language_id = sqlGetWordIDLanguage(word_defs[i].word);
        int english_id = sqlGetWordIDEnglish(word_defs[i].keyword);

        if(language_id != english_id || expected_returns[i] != english_id) {
            printf("language_id != english_id or expected_return != english_id\n");
            printf("language_id: %d\n", language_id);
            printf("english_id: %d\n", english_id);
            printf("expected_return: %d\n", expected_returns[i]);

            wordDefinitionStr(&word_defs[i], output, sizeof(output) / sizeof(output[0]));

            printf("%s\n", output);
        }
    }

    // Check words.

    {
        WordDefinition word_def = {"Weh", "PARTITION", "CARD", "It is non sense."};
        sqlUpdateWord(sqlGetWordIDEnglish("CART"), &word_def);
    }

    sqlDeinit();

    return problem;
}
