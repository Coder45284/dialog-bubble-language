#include "database.h"

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

    problem |= noInitTest("BEFORE dbInit");

    // Do not need to write to disk.
    dbInit(":memory:");

    problem |= successfullInitTest("FIRST successfullInitTest");

    dbDeinit();

    if(problem == 0) {
        problem |= noInitTest("AFTER dbDeinit");

        dbInit(":memory:");

        problem |= successfullInitTest("SECOND successfullInitTest");

        dbDeinit();
    }

    return problem;
}

int noInitTest(const char *const text) {
    DBWordDefinition word_def = {"QeeSoeWee", "VERB", "RUN", "The action of running."};
    int problem = 0;

    printf("noInitTest %s\n", text);

    STATUS_CHECK(DB_NOT_INIT, "DB_NOT_INIT", dbGetWordIDLanguage("QeeSoeWee"), "dbGetWordIDLanguage");
    STATUS_CHECK(DB_NOT_INIT, "DB_NOT_INIT", dbGetWordIDEnglish("RUN"), "dbGetWordIDEnglish");

    STATUS_CHECK(DB_NOT_INIT, "DB_NOT_INIT", dbAddWord(&word_def), "dbAddWord");
    STATUS_CHECK(DB_NOT_INIT, "DB_NOT_INIT", dbUpdateWord(1, &word_def), "dbUpdateWord");
    STATUS_CHECK(DB_NOT_INIT, "DB_NOT_INIT", dbGetWord(1, &word_def), "dbGetWord");
    STATUS_CHECK(DB_NOT_INIT, "DB_NOT_INIT", dbRemoveWord(1), "dbRemoveWord");

    return problem;
}

int successfullInitTest(const char *const text) {
    int problem = 0;

    printf("successfullInitTest %s\n", text);

    DBWordDefinition word_defs[5] = {
        {"QeeSoeWee", "VERB", "RUN", "The action of running."},
        {"QeeSeeWel", "NOUN;ADJECTIVE", "RED", "The color red."},
        {"QoeQoeWeh", "NOUN", "CART", "A cart like a shopping cart."},
        {"ToeQeeWee", "ADVERB;ADJECTIVE", "SLOW", "Slow movement or to describe something with slow movement."},
        {"@", "@", "@", "@"}
    };

    int expected_returns[5] = {1, 2, 3, 4, DB_DNE};

    for(int i = 0; i < 4; i++) {
        const char word[2] = {'1' + i, '\0'};

        STATUS_CHECK(1 + i, word, dbAddWord(&word_defs[i]), "dbAddWord");
    }

    char output[0x100];
    DBWordDefinition definition_test;

    // Sanity checks
    for(int i = 0; i < 5; i++) {
        int language_id = dbGetWordIDLanguage(word_defs[i].word);
        int english_id = dbGetWordIDEnglish(word_defs[i].keyword);

        if(language_id != english_id) {
            printf("language_id != english_id\n");
            printf("language_id: %d\n", language_id);
            printf("english_id: %d\n", english_id);

            dbWordDefinitionStr(&word_defs[i], output, sizeof(output) / sizeof(output[0]));
            printf("%s\n", output);
        }

        if(expected_returns[i] != english_id) {
            printf("expected_return != english_id\n");
            printf("expected_return: %d\n", expected_returns[i]);
            printf("english_id: %d\n", english_id);

            dbWordDefinitionStr(&word_defs[i], output, sizeof(output) / sizeof(output[0]));
            printf("%s\n", output);

        }

        if(expected_returns[i] > DB_DNE) {
            int result = dbGetWord(english_id, &definition_test);

            STATUS_CHECK(DB_SUCCESS, "DB_SUCCESS", result, "dbGetWord");

            if(result == DB_SUCCESS) {
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
            STATUS_CHECK(DB_DNE, "DB_DNE", dbGetWord(english_id, &definition_test), "dbGetWord");
        }
    }

    // Update tests.
    {
        const DBWordDefinition expected_def = {"Weh", "PARTITION", "CARD", "It is non sense."};

        const int word_id = dbGetWordIDEnglish(word_defs[1].keyword);

        STATUS_CHECK(DB_DNE,     "DB_DNE",     dbUpdateWord(29,      &expected_def), "dbUpdateWord");
        STATUS_CHECK(DB_SUCCESS, "DB_SUCCESS", dbUpdateWord(word_id, &expected_def), "dbUpdateWord");

        int result = dbGetWord(word_id, &word_defs[1]);

        STATUS_CHECK(DB_SUCCESS, "DB_SUCCESS", result, "dbGetWord from updated word");

        for(int i = 0; i < 4; i++) {
            int result = dbGetWord(dbGetWordIDEnglish(word_defs[i].keyword), &definition_test);

            STATUS_CHECK(DB_SUCCESS, "update DB_SUCCESS", result, "dbGetWord");

            if(result == DB_SUCCESS) {
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
        const int word_id = dbGetWordIDEnglish(word_defs[2].keyword);

        STATUS_CHECK(DB_DNE,     "DB_DNE",     dbRemoveWord(29),      "dbRemoveWord");
        STATUS_CHECK(DB_SUCCESS, "DB_SUCCESS", dbRemoveWord(word_id), "dbRemoveWord");

        for(int i = 0; i < 4; i++) {
            int result = dbGetWord(dbGetWordIDEnglish(word_defs[i].keyword), &definition_test);

            if(i == 2)
                continue;

            STATUS_CHECK(DB_SUCCESS, "remove DB_SUCCESS", result, "dbGetWord");

            if(result == DB_SUCCESS) {
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
        const int result = dbAddWord(&word_defs[2]);
        if(result <= DB_DNE) {
            dbWordDefinitionStr(&word_defs[2], output, sizeof(output) / sizeof(output[0]));
            printf("Definition cannot be placed back: Code %d. For definition\n%s\n", result, output);
            problem |= 1;
        }
    }

    // I am very sure that there are still bugs with SQL. Rather than fix them now, I will fix them as I detect them. This way I can be sure that I fixed them.
    // Test Add same language word test.
    {
        DBWordDefinition word_defs[2] = {
            {"QeeSoeQee", "VERB", "STANDING", "The action of running."},
            {"QeeSoeQee", "NOUN", "BLA", "An word of nothingness."}
        };

        const int result = dbAddWord(&word_defs[0]);
        if(result <= DB_DNE) {
            dbWordDefinitionStr(&word_defs[0], output, sizeof(output) / sizeof(output[0]));
            printf("Same Language Word Case:This should have not had failed: Code %d. For definition\n%s\n", result, output);
            problem |= 1;
        }
        STATUS_CHECK(DB_ONLY_ONE_ENTRY, "Same Language Word Case: DB_ONLY_ONE_ENTRY", dbAddWord(&word_defs[1]), "dbAddWord");
    }
    // Test Add same English keyword test.
    {
        DBWordDefinition word_defs[2] = {
            {"QeeToeTee", "VERB", "WAITING", "The action of running."},
            {"QeeToeTel", "NOUN", "WAITING", "An word of nothingness."}
        };

        const int result = dbAddWord(&word_defs[0]);
        if(result <= DB_DNE) {
            dbWordDefinitionStr(&word_defs[0], output, sizeof(output) / sizeof(output[0]));
            printf("Same English Word Case:This should have not had failed: Code %d. For definition\n%s\n", result, output);
            problem |= 1;
        }
        STATUS_CHECK(DB_ONLY_ONE_ENTRY, "Same English Word Case: DB_ONLY_ONE_ENTRY", dbAddWord(&word_defs[1]), "dbAddWord");
    }
    // Test Update same language word test.
    {
        DBWordDefinition word_defs[2] = {
            {"QeeToeTeeQee", "VERB", "WAITINGS", "The action of running."},
            {"TeeToeTeeQee", "NOUN", "BLAY", "An word of nothingness."}
        };

        const int id = dbAddWord(&word_defs[0]);
        if(id <= DB_DNE) {
            dbWordDefinitionStr(&word_defs[0], output, sizeof(output) / sizeof(output[0]));
            printf("At %d. Same Language Word Update Case:This should have not had failed: Code %d. For definition\n%s\n", 0, id, output);
            problem |= 1;
        }
        const int second_id = dbAddWord(&word_defs[1]);
        if(id <= DB_DNE) {
            dbWordDefinitionStr(&word_defs[1], output, sizeof(output) / sizeof(output[0]));
            printf("At %d. Same Language Word Update Case:This should have not had failed: Code %d. For definition\n%s\n", 1, id, output);
            problem |= 1;
        }

        word_defs[1].word[0] = word_defs[0].word[0];

        STATUS_CHECK(DB_ONLY_ONE_ENTRY, "Same Language Word Update DB_ONLY_ONE_ENTRY", dbUpdateWord(second_id, &word_defs[1]), "dbUpdateWord");
    }
    // Test Update same English keyword test.
    {
        DBWordDefinition word_defs[2] = {
            {"QeeToeTeeTel", "VERB",      "MAD", "State of anger."},
            {"TeeToeTeeTeh", "ADJECTIVE", "BAD", "Opsite of good."}
        };

        const int id = dbAddWord(&word_defs[0]);
        if(id <= DB_DNE) {
            dbWordDefinitionStr(&word_defs[0], output, sizeof(output) / sizeof(output[0]));
            printf("At %d. Same Language Word Update Case:This should have not had failed: Code %d. For definition\n%s\n", 0, id, output);
            problem |= 1;
        }
        const int second_id = dbAddWord(&word_defs[1]);
        if(id <= DB_DNE) {
            dbWordDefinitionStr(&word_defs[1], output, sizeof(output) / sizeof(output[0]));
            printf("At %d. Same Language Word Update Case:This should have not had failed: Code %d. For definition\n%s\n", 1, id, output);
            problem |= 1;
        }

        word_defs[1].keyword[0] = word_defs[0].keyword[0];

        STATUS_CHECK(DB_ONLY_ONE_ENTRY, "Same Language Keyword Update DB_ONLY_ONE_ENTRY", dbUpdateWord(second_id, &word_defs[1]), "dbUpdateWord");
    }

    // Find a way to make a stability test on both tables. Each table should have the same amount of entries.
    // Check the stability of the table.
    if(dbIsDatabaseOkay(output, sizeof(output) / sizeof(output[0])) != DB_SUCCESS) {
        printf("Database %s", output);
        problem |= 1;
    }

    return problem;
}
