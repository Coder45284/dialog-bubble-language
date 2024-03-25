#include "sql.h"

#include <sqlite3.h>

#include <stddef.h>
#include <stdio.h>

static int sqlLiteCallback(void* ignored, int argc, char** argv, char** collumn_name);

sqlite3 *database = NULL;

sqlite3_stmt *sql_get_language_word_to_id_code = NULL;
sqlite3_stmt *sql_get_english_word_to_id_code = NULL;
sqlite3_stmt *sql_get_entry_dictionary_id_code = NULL;
sqlite3_stmt *sql_insert_dictionary_code = NULL;
sqlite3_stmt *sql_insert_english_translation_code = NULL;
sqlite3_stmt *sql_update_dictionary_code = NULL;
sqlite3_stmt *sql_update_english_translation_code = NULL;

int sqlInit() {
    int db_return;
    char *sql_error_mesg = NULL;

    db_return = sqlite3_open("database.db", &database);

    if(db_return) {
        printf("Database can not be opened \"%s\"\n", sqlite3_errmsg(database));
        sqlite3_close(database);
        return 1;
    }

    const char SQL_BUILD_DICTIONARY[] = "CREATE TABLE DICTIONARY("
        "W_ID "         "INTEGER PRIMARY KEY,"
        "WORD "         "CHAR(128) UNIQUE,"
        "PARTS_OF_SPEECH CHAR(64))";

    db_return = sqlite3_exec(database, SQL_BUILD_DICTIONARY, sqlLiteCallback, 0, &sql_error_mesg);

    if(db_return != SQLITE_OK) {
        printf("Sqlite3 error: %s\n", sql_error_mesg);
        sqlite3_free(sql_error_mesg);
    }

    const char SQL_BUILD_ENGLISH_TRANSLATION[] = "CREATE TABLE ENGLISH_TRANSLATION("
        "W_ID "    "INT PRIMARY KEY NOT NULL,"
        "KEYWORD " "CHAR(64),"
        "DEFINITION TEXT)";

    db_return = sqlite3_exec(database, SQL_BUILD_ENGLISH_TRANSLATION, sqlLiteCallback, 0, &sql_error_mesg);

    if(db_return != SQLITE_OK) {
        printf("Sqlite3 error: %s\n", sql_error_mesg);
        sqlite3_free(sql_error_mesg);
    }

    db_return = sqlite3_prepare_v2(database, "SELECT W_ID FROM DICTIONARY WHERE WORD=?1;", -1, &sql_get_language_word_to_id_code, NULL);
    if(db_return != SQLITE_OK)
        printf("Sqlite3 prepare error: %s\n", sqlite3_errstr(db_return));

    db_return = sqlite3_prepare_v2(database, "SELECT W_ID FROM ENGLISH_TRANSLATION WHERE KEYWORD=?1;", -1, &sql_get_english_word_to_id_code, NULL);
    if(db_return != SQLITE_OK)
        printf("Sqlite3 prepare error: %s\n", sqlite3_errstr(db_return));

    db_return = sqlite3_prepare_v2(database, "SELECT W_ID FROM DICTIONARY WHERE WORD=?1 AND PARTS_OF_SPEECH=?2;", -1, &sql_get_entry_dictionary_id_code, NULL);
    if(db_return != SQLITE_OK)
        printf("Sqlite3 prepare error: %s\n", sqlite3_errstr(db_return));

    db_return = sqlite3_prepare_v2(database, "INSERT INTO DICTIONARY(WORD,PARTS_OF_SPEECH)VALUES(?1,?2);", -1, &sql_insert_dictionary_code, NULL);
    if(db_return != SQLITE_OK)
        printf("Sqlite3 prepare error: %s\n", sqlite3_errstr(db_return));

    db_return = sqlite3_prepare_v2(database, "INSERT INTO ENGLISH_TRANSLATION VALUES(?1,?2,?3);", -1, &sql_insert_english_translation_code, NULL);
    if(db_return != SQLITE_OK)
        printf("Sqlite3 prepare error: %s\n", sqlite3_errstr(db_return));

    db_return = sqlite3_prepare_v2(database, "UPDATE DICTIONARY SET WORD=?1,PARTS_OF_SPEECH=?2 WHERE W_ID=?3;", -1, &sql_update_dictionary_code, NULL);
    if(db_return != SQLITE_OK)
        printf("Sqlite3 prepare error: %s\n", sqlite3_errstr(db_return));

    db_return = sqlite3_prepare_v2(database, "UPDATE ENGLISH_TRANSLATION SET KEYWORD=?1,DEFINITION=?2 WHERE W_ID=?3;", -1, &sql_update_english_translation_code, NULL);
    if(db_return != SQLITE_OK)
        printf("Sqlite3 prepare error: %s\n", sqlite3_errstr(db_return));

    sqlAddWord("QeeSoeWee", "VERB",              "RUN", "The action of running.");
    sqlAddWord("QeeSeeWel", "NOUN;ADJECTIVE",    "RED", "The color red.");
    sqlAddWord("QoeQoeWeh", "NOUN",             "CART", "A cart like a shopping cart.");
    sqlAddWord("ToeQeeWee", "ADVERB;ADJECTIVE", "SLOW", "Slow movement or to describe something with slow movement.");

    return 0;
}

void sqlDeinit() {

    sqlite3_finalize(sql_get_language_word_to_id_code);
    sqlite3_finalize(sql_get_english_word_to_id_code);
    sqlite3_finalize(sql_get_entry_dictionary_id_code);
    sqlite3_finalize(sql_insert_dictionary_code);
    sqlite3_finalize(sql_insert_english_translation_code);
    sqlite3_finalize(sql_update_dictionary_code);
    sqlite3_finalize(sql_update_english_translation_code);

    sqlite3_close(database);
}

int sqlGetWordIDLanguage(const char *const word) {
    int db_return;
    sqlite3_int64 id_number = 0;

    if(sql_get_language_word_to_id_code != NULL) {
        sqlite3_bind_text(sql_get_language_word_to_id_code, 1, word, -1, NULL);\

        db_return = sqlite3_step(sql_get_language_word_to_id_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_get_language_word_to_id_code);
        }

        if(db_return == SQLITE_ROW)
            id_number = sqlite3_column_int64(sql_get_language_word_to_id_code, 0);

        db_return = sqlite3_step(sql_get_language_word_to_id_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_get_language_word_to_id_code);
        }

        if(db_return != SQLITE_DONE) {
            printf("sqlGetWordIDLanguage prepare error: %s\n", sqlite3_errstr(db_return) );
        }

        sqlite3_reset(sql_get_language_word_to_id_code);
    }

    return id_number;
}

int sqlGetWordIDEnglish(const char *const word) {
    int db_return;
    sqlite3_int64 id_number = 0;

    if(sql_get_english_word_to_id_code != NULL) {
        sqlite3_bind_text(sql_get_english_word_to_id_code, 1, word, -1, NULL);\

        db_return = sqlite3_step(sql_get_english_word_to_id_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_get_english_word_to_id_code);
        }

        if(db_return == SQLITE_ROW)
            id_number = sqlite3_column_int64(sql_get_english_word_to_id_code, 0);

        db_return = sqlite3_step(sql_get_english_word_to_id_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_get_english_word_to_id_code);
        }

        if(db_return != SQLITE_DONE) {
            printf("sqlGetWordIDEnglish prepare error: %s\n", sqlite3_errstr(db_return) );
        }

        sqlite3_reset(sql_get_english_word_to_id_code);
    }

    return id_number;
}

int sqlAddWord(const char *const word, const char *const parts_of_speech, const char *const english_keyword, const char *const english_definition) {
    int db_return;

    // Make sure the prepared statements are allocated.
    if(sql_insert_dictionary_code == NULL || sql_get_entry_dictionary_id_code == NULL || sql_insert_english_translation_code == NULL)
        return -1; // Missing prepared statements

    // Make sure that the word does not exist first.
    if(sqlGetWordIDLanguage(word) != 0)
        return -2; // Word already exists or mi

    {
        sqlite3_bind_text( sql_insert_dictionary_code, 1, word, -1, NULL);
        sqlite3_bind_text( sql_insert_dictionary_code, 2, parts_of_speech, -1, NULL);

        db_return = sqlite3_step(sql_insert_dictionary_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_insert_dictionary_code);
        }

        if(db_return != SQLITE_DONE) {
            printf("Sqlite3 prepare error: %s\n", sqlite3_errstr(db_return) );
        }

        sqlite3_reset(sql_insert_dictionary_code);
    }

    sqlite3_int64 id_number = 0;

    {
        sqlite3_bind_text(sql_get_entry_dictionary_id_code, 1, word, -1, NULL);
        sqlite3_bind_text(sql_get_entry_dictionary_id_code, 2, parts_of_speech, -1, NULL);

        db_return = sqlite3_step(sql_get_entry_dictionary_id_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_get_entry_dictionary_id_code);
        }

        if(db_return == SQLITE_ROW)
            id_number = sqlite3_column_int64(sql_get_entry_dictionary_id_code, 0);

        db_return = sqlite3_step(sql_get_entry_dictionary_id_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_get_entry_dictionary_id_code);
        }

        if(db_return != SQLITE_DONE) {
            printf("Sqlite3 prepare error: %s\n", sqlite3_errstr(db_return) );
        }

        sqlite3_reset(sql_get_entry_dictionary_id_code);
    }

    {
        sqlite3_bind_int64(sql_insert_english_translation_code, 1, id_number);
        sqlite3_bind_text( sql_insert_english_translation_code, 2, english_keyword, -1, NULL);
        sqlite3_bind_text( sql_insert_english_translation_code, 3, english_definition, -1, NULL);

        db_return = sqlite3_step(sql_insert_english_translation_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_insert_english_translation_code);
        }

        if(db_return != SQLITE_DONE) {
            printf("Sqlite3 prepare error: %s\n", sqlite3_errstr(db_return) );
        }

        sqlite3_reset(sql_insert_english_translation_code);
    }
    return id_number;
}

static int sqlLiteCallback(void* ignored, int argc, char** argv, char** collumn_name) {
    for(int i = 0; i < argc; i++) {
        printf("%s: ", collumn_name[i]);
        if(argv[i] != NULL)
            printf("%s\n", argv);
        else
            printf("NULL\n", argv);
    }
    printf("\n");

    return 0;
}
