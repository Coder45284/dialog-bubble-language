#include "sql.h"

#include <stdio.h>
#include <string.h>

#define STATUS_CHECK(expected, expected_text, function, word)\
{\
    int status = function;\
    \
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
    WordDefinition definition_test;

    // Sanity checks
    for(int i = 0; i < 5; i++) {
        int language_id = sqlGetWordIDLanguage(word_defs[i].word);
        int english_id = sqlGetWordIDEnglish(word_defs[i].keyword);

        if(language_id != english_id) {
            printf("language_id != english_id\n");
            printf("language_id: %d\n", language_id);
            printf("english_id: %d\n", english_id);

            wordDefinitionStr(&word_defs[i], output, sizeof(output) / sizeof(output[0]));
            printf("%s\n", output);
        }

        if(expected_returns[i] != english_id) {
            printf("expected_return != english_id\n");
            printf("expected_return: %d\n", expected_returns[i]);
            printf("english_id: %d\n", english_id);

            wordDefinitionStr(&word_defs[i], output, sizeof(output) / sizeof(output[0]));
            printf("%s\n", output);

        }

        if(expected_returns[i] > SQL_DNE) {
            int result = sqlGetWord(english_id, &definition_test);

            STATUS_CHECK(SQL_SUCCESS, "SQL_SUCCESS", result, "sqlGetWord");

            if(result == SQL_SUCCESS) {
                if(strncmp(word_defs[i].word, definition_test.word, sizeof(definition_test.word) / sizeof(definition_test.word[0])) != 0) {
                    printf("index %d\nword_defs[i].word:\"%s\" != \"%s\":definition_test.word\n", i, word_defs[i].word, definition_test.word);
                    problem |= 1;
                }
                if(strncmp(word_defs[i].parts_of_speech, definition_test.parts_of_speech, sizeof(definition_test.parts_of_speech) / sizeof(definition_test.parts_of_speech[0])) != 0) {
                    printf("index %d\nword_defs[i].parts_of_speech:\"%s\" != \"%s\":definition_test.parts_of_speech\n", i, word_defs[i].parts_of_speech, definition_test.parts_of_speech);
                    problem |= 1;
                }
                if(strncmp(word_defs[i].keyword, definition_test.keyword, sizeof(definition_test.keyword) / sizeof(definition_test.keyword[0])) != 0) {
                    printf("index %d\nword_defs[i].keyword:\"%s\" != \"%s\":definition_test.keyword\n", i, word_defs[i].keyword, definition_test.keyword);
                    problem |= 1;
                }
                if(strncmp(word_defs[i].definition, definition_test.definition, sizeof(definition_test.definition) / sizeof(definition_test.definition[0])) != 0) {
                    printf("index %d\nword_defs[i].definition:\"%s\" != \"%s\":definition_test.definition\n", i, word_defs[i].definition, definition_test.definition);
                    problem |= 1;
                }
            }
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
