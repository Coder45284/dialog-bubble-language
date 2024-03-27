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

int noInitTest(const char *const text);
int successfullInitTest(const char *const text);

int main() {
    int problem = 0;

    problem |= noInitTest("BEFORE sqlInit");

    // Do not need to write to disk.
    sqlInit(":memory:");

    problem |= successfullInitTest("FIRST successfullInitTest");

    sqlDeinit();

    if(problem == 0) {
        problem |= noInitTest("AFTER sqlDeinit");

        sqlInit(":memory:");

        problem |= successfullInitTest("SECOND successfullInitTest");

        sqlDeinit();
    }

    return problem;
}

int noInitTest(const char *const text) {
    int problem = 0;

    printf("noInitTest %s\n", text);

    STATUS_CHECK(SQL_NOT_INIT, "SQL_NOT_INIT", sqlGetWordIDLanguage("QeeSoeWee"), "sqlGetWordIDLanguage");
    STATUS_CHECK(SQL_NOT_INIT, "SQL_NOT_INIT", sqlGetWordIDEnglish("RUN"), "sqlGetWordIDEnglish");

    {
        WordDefinition word_def = {"QeeSoeWee", "VERB", "RUN", "The action of running."};

        STATUS_CHECK(SQL_NOT_INIT, "SQL_NOT_INIT", sqlAddWord(&word_def), "sqlAddWord");
        STATUS_CHECK(SQL_NOT_INIT, "SQL_NOT_INIT", sqlUpdateWord(1, &word_def), "sqlUpdateWord");
        STATUS_CHECK(SQL_NOT_INIT, "SQL_NOT_INIT", sqlGetWord(1, &word_def), "sqlGetWord");
        STATUS_CHECK(SQL_NOT_INIT, "SQL_NOT_INIT", sqlRemoveWord(1), "sqlRemoveWord");
    }

    return problem;
}

int successfullInitTest(const char *const text) {
    int problem = 0;

    printf("successfullInitTest %s\n", text);

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
                    printf("word_defs[%d].word:\"%s\" != \"%s\":definition_test.word\n", i, word_defs[i].word, definition_test.word);
                    problem |= 1;
                }
                if(strncmp(word_defs[i].parts_of_speech, definition_test.parts_of_speech, sizeof(definition_test.parts_of_speech) / sizeof(definition_test.parts_of_speech[0])) != 0) {
                    printf("word_defs[%d].parts_of_speech:\"%s\" != \"%s\":definition_test.parts_of_speech\n", i, word_defs[i].parts_of_speech, definition_test.parts_of_speech);
                    problem |= 1;
                }
                if(strncmp(word_defs[i].keyword, definition_test.keyword, sizeof(definition_test.keyword) / sizeof(definition_test.keyword[0])) != 0) {
                    printf("word_defs[%d].keyword:\"%s\" != \"%s\":definition_test.keyword\n", i, word_defs[i].keyword, definition_test.keyword);
                    problem |= 1;
                }
                if(strncmp(word_defs[i].definition, definition_test.definition, sizeof(definition_test.definition) / sizeof(definition_test.definition[0])) != 0) {
                    printf("word_defs[%d].definition:\"%s\" != \"%s\":definition_test.definition\n", i, word_defs[i].definition, definition_test.definition);
                    problem |= 1;
                }
            }
        }
        else {
            STATUS_CHECK(SQL_DNE, "SQL_DNE", sqlGetWord(english_id, &definition_test), "sqlGetWord");
        }
    }

    // Update tests.
    {
        const WordDefinition expected_def = {"Weh", "PARTITION", "CARD", "It is non sense."};

        const int word_id = sqlGetWordIDEnglish(word_defs[1].keyword);

        STATUS_CHECK(SQL_DNE,     "SQL_DNE",     sqlUpdateWord(29,      &expected_def), "sqlUpdateWord");
        STATUS_CHECK(SQL_SUCCESS, "SQL_SUCCESS", sqlUpdateWord(word_id, &expected_def), "sqlUpdateWord");

        int result = sqlGetWord(word_id, &word_defs[1]);

        STATUS_CHECK(SQL_SUCCESS, "SQL_SUCCESS", result, "sqlGetWord from updated word");

        for(int i = 0; i < 4; i++) {
            int result = sqlGetWord(sqlGetWordIDEnglish(word_defs[i].keyword), &definition_test);

            STATUS_CHECK(SQL_SUCCESS, "update SQL_SUCCESS", result, "sqlGetWord");

            if(result == SQL_SUCCESS) {
                if(strncmp(word_defs[i].word, definition_test.word, sizeof(definition_test.word) / sizeof(definition_test.word[0])) != 0) {
                    printf("update check word_defs[%d].word:\"%s\" != \"%s\":definition_test.word\n", i, word_defs[i].word, definition_test.word);
                    problem |= 1;
                }
                if(strncmp(word_defs[i].parts_of_speech, definition_test.parts_of_speech, sizeof(definition_test.parts_of_speech) / sizeof(definition_test.parts_of_speech[0])) != 0) {
                    printf("update check word_defs[%d].parts_of_speech:\"%s\" != \"%s\":definition_test.parts_of_speech\n", i, word_defs[i].parts_of_speech, definition_test.parts_of_speech);
                    problem |= 1;
                }
                if(strncmp(word_defs[i].keyword, definition_test.keyword, sizeof(definition_test.keyword) / sizeof(definition_test.keyword[0])) != 0) {
                    printf("update check word_defs[%d].keyword:\"%s\" != \"%s\":definition_test.keyword\n", i, word_defs[i].keyword, definition_test.keyword);
                    problem |= 1;
                }
                if(strncmp(word_defs[i].definition, definition_test.definition, sizeof(definition_test.definition) / sizeof(definition_test.definition[0])) != 0) {
                    printf("update check word_defs[%d].definition:\"%s\" != \"%s\":definition_test.definition\n", i, word_defs[i].definition, definition_test.definition);
                    problem |= 1;
                }
            }
        }
    }

    // Remove tests.
    {
        const int word_id = sqlGetWordIDEnglish(word_defs[2].keyword);

        STATUS_CHECK(SQL_DNE,     "SQL_DNE",     sqlRemoveWord(29),      "sqlRemoveWord");
        STATUS_CHECK(SQL_SUCCESS, "SQL_SUCCESS", sqlRemoveWord(word_id), "sqlRemoveWord");

        for(int i = 0; i < 4; i++) {
            int result = sqlGetWord(sqlGetWordIDEnglish(word_defs[i].keyword), &definition_test);

            if(i == 2)
                continue;

            STATUS_CHECK(SQL_SUCCESS, "remove SQL_SUCCESS", result, "sqlGetWord");

            if(result == SQL_SUCCESS) {
                if(strncmp(word_defs[i].word, definition_test.word, sizeof(definition_test.word) / sizeof(definition_test.word[0])) != 0) {
                    printf("remove check word_defs[%d].word:\"%s\" != \"%s\":definition_test.word\n", i, word_defs[i].word, definition_test.word);
                    problem |= 1;
                }
                if(strncmp(word_defs[i].parts_of_speech, definition_test.parts_of_speech, sizeof(definition_test.parts_of_speech) / sizeof(definition_test.parts_of_speech[0])) != 0) {
                    printf("remove check word_defs[%d].parts_of_speech:\"%s\" != \"%s\":definition_test.parts_of_speech\n", i, word_defs[i].parts_of_speech, definition_test.parts_of_speech);
                    problem |= 1;
                }
                if(strncmp(word_defs[i].keyword, definition_test.keyword, sizeof(definition_test.keyword) / sizeof(definition_test.keyword[0])) != 0) {
                    printf("remove check word_defs[%d].keyword:\"%s\" != \"%s\":definition_test.keyword\n", i, word_defs[i].keyword, definition_test.keyword);
                    problem |= 1;
                }
                if(strncmp(word_defs[i].definition, definition_test.definition, sizeof(definition_test.definition) / sizeof(definition_test.definition[0])) != 0) {
                    printf("remove check word_defs[%d].definition:\"%s\" != \"%s\":definition_test.definition\n", i, word_defs[i].definition, definition_test.definition);
                    problem |= 1;
                }
            }
        }

        // Add this word back
        const int result = sqlAddWord(&word_defs[2]);
        if(result <= SQL_DNE) {
            wordDefinitionStr(&word_defs[2], output, sizeof(output) / sizeof(output[0]));
            printf("Definition cannot be placed back: Code %d. For definition\n%s\n", result, output);
            problem |= 1;
        }
    }

    // I am very sure that there are still bugs with SQL. Rather than fix them now, I will fix them as I detect them. This way I can be sure that I fixed them.
    // TODO Test Add in same keyword and name case.
    // TODO Test Update in same keyword and name case.

    return problem;
}
