#include "sql.h"

#include <sqlite3.h>

#include <stddef.h>
#include <string.h>
#include <stdio.h>

static void sqlLitePrepare(const char *const statement, sqlite3_stmt **code);
static int sqlLiteDoesTableExist(const char *const table_name);
static int sqlLiteCallback(void* ignored, int argc, char** argv, char** collumn_name);

static sqlite3 *database = NULL;

static sqlite3_stmt *sql_check_for_table_code = NULL;
static sqlite3_stmt *sql_get_language_word_to_id_code = NULL;
static sqlite3_stmt *sql_get_english_word_to_id_code = NULL;
static sqlite3_stmt *sql_get_entry_dictionary_id_code = NULL;
static sqlite3_stmt *sql_insert_dictionary_code = NULL;
static sqlite3_stmt *sql_insert_english_translation_code = NULL;
static sqlite3_stmt *sql_update_dictionary_code = NULL;
static sqlite3_stmt *sql_update_english_translation_code = NULL;
static sqlite3_stmt *sql_get_dictionary_entry_code = NULL;
static sqlite3_stmt *sql_get_english_translation_entry_code = NULL;
static sqlite3_stmt *sql_delete_dictionary_entry_code = NULL;
static sqlite3_stmt *sql_delete_english_translation_entry_code = NULL;
static sqlite3_stmt *sql_count_dictionary_entries_code = NULL;
static sqlite3_stmt *sql_count_english_translation_entries_code = NULL;

int sqlInit(const char *const path) {
    int db_return;
    char *sql_error_mesg = NULL;

    db_return = sqlite3_open(path, &database);

    if(db_return) {
        printf("Database can not be opened \"%s\"\n", sqlite3_errmsg(database));
        sqlite3_close(database);
        return 1;
    }

    sqlLitePrepare("SELECT * FROM sqlite_master WHERE type='table' AND tbl_name=?1", &sql_check_for_table_code);

    if(!sqlLiteDoesTableExist("DICTIONARY")) {
        const char SQL_BUILD_DICTIONARY[] = "CREATE TABLE DICTIONARY("
            "W_ID "         "INTEGER PRIMARY KEY,"
            "WORD "         "CHAR(127) UNIQUE,"
            "PARTS_OF_SPEECH CHAR(63))";

        db_return = sqlite3_exec(database, SQL_BUILD_DICTIONARY, sqlLiteCallback, 0, &sql_error_mesg);

        if(db_return != SQLITE_OK) {
            printf("Sqlite3 error: %s\n", sql_error_mesg);
            sqlite3_free(sql_error_mesg);
        }
    }

    if(!sqlLiteDoesTableExist("ENGLISH_TRANSLATION")) {
        const char SQL_BUILD_ENGLISH_TRANSLATION[] = "CREATE TABLE ENGLISH_TRANSLATION("
            "W_ID "    "INT,"
            "KEYWORD " "CHAR(63) UNIQUE,"
            "DEFINITION VARCHAR,"
            "FOREIGN KEY (W_ID) REFERENCES DICTIONARY(W_ID))";

        db_return = sqlite3_exec(database, SQL_BUILD_ENGLISH_TRANSLATION, sqlLiteCallback, 0, &sql_error_mesg);

        if(db_return != SQLITE_OK) {
            printf("Sqlite3 error: %s\n", sql_error_mesg);
            sqlite3_free(sql_error_mesg);
        }
    }

    sqlLitePrepare("SELECT W_ID FROM DICTIONARY WHERE WORD=?1;", &sql_get_language_word_to_id_code);
    sqlLitePrepare("SELECT W_ID FROM ENGLISH_TRANSLATION WHERE KEYWORD=?1;", &sql_get_english_word_to_id_code);
    sqlLitePrepare("SELECT W_ID FROM DICTIONARY WHERE WORD=?1 AND PARTS_OF_SPEECH=?2;", &sql_get_entry_dictionary_id_code);
    sqlLitePrepare("INSERT INTO DICTIONARY(WORD,PARTS_OF_SPEECH)VALUES(?1,?2);", &sql_insert_dictionary_code);
    sqlLitePrepare("INSERT INTO ENGLISH_TRANSLATION VALUES(?1,?2,?3);", &sql_insert_english_translation_code);
    sqlLitePrepare("UPDATE DICTIONARY SET WORD=?1,PARTS_OF_SPEECH=?2 WHERE W_ID=?3;", &sql_update_dictionary_code);
    sqlLitePrepare("UPDATE ENGLISH_TRANSLATION SET KEYWORD=?1,DEFINITION=?2 WHERE W_ID=?3;", &sql_update_english_translation_code);
    sqlLitePrepare("SELECT WORD,PARTS_OF_SPEECH FROM DICTIONARY WHERE W_ID=?1;", &sql_get_dictionary_entry_code);
    sqlLitePrepare("SELECT KEYWORD,DEFINITION FROM ENGLISH_TRANSLATION WHERE W_ID=?1;", &sql_get_english_translation_entry_code);
    sqlLitePrepare("DELETE FROM DICTIONARY WHERE W_ID=?1;", &sql_delete_dictionary_entry_code);
    sqlLitePrepare("DELETE FROM ENGLISH_TRANSLATION WHERE W_ID=?1;", &sql_delete_english_translation_entry_code);
    sqlLitePrepare("SELECT COUNT(*) FROM DICTIONARY;", &sql_count_dictionary_entries_code);
    sqlLitePrepare("SELECT COUNT(*) FROM ENGLISH_TRANSLATION;", &sql_count_english_translation_entries_code);

    return 0;
}

void sqlDeinit() {

    sqlite3_finalize(sql_check_for_table_code);
    sql_check_for_table_code = NULL;
    sqlite3_finalize(sql_get_language_word_to_id_code);
    sql_get_language_word_to_id_code = NULL;
    sqlite3_finalize(sql_get_english_word_to_id_code);
    sql_get_english_word_to_id_code = NULL;
    sqlite3_finalize(sql_get_entry_dictionary_id_code);
    sql_get_entry_dictionary_id_code = NULL;
    sqlite3_finalize(sql_insert_dictionary_code);
    sql_insert_dictionary_code = NULL;
    sqlite3_finalize(sql_insert_english_translation_code);
    sql_insert_english_translation_code = NULL;
    sqlite3_finalize(sql_update_dictionary_code);
    sql_update_dictionary_code = NULL;
    sqlite3_finalize(sql_update_english_translation_code);
    sql_update_english_translation_code = NULL;
    sqlite3_finalize(sql_get_dictionary_entry_code);
    sql_get_dictionary_entry_code = NULL;
    sqlite3_finalize(sql_get_english_translation_entry_code);
    sql_get_english_translation_entry_code = NULL;
    sqlite3_finalize(sql_delete_dictionary_entry_code);
    sql_delete_dictionary_entry_code = NULL;
    sqlite3_finalize(sql_delete_english_translation_entry_code);
    sql_delete_english_translation_entry_code = NULL;
    sqlite3_finalize(sql_count_dictionary_entries_code);
    sql_count_dictionary_entries_code = NULL;
    sqlite3_finalize(sql_count_english_translation_entries_code);
    sql_count_english_translation_entries_code = NULL;

    sqlite3_close(database);
    database = NULL;
}

int sqlGetWordIDLanguage(const char *const word) {
    int db_return;
    sqlite3_int64 id_number = SQL_DNE;

    // Make sure the prepared statements are allocated.
    if(sql_get_language_word_to_id_code == NULL)
        return SQL_NOT_INIT; // Missing prepared statements

    {
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
    sqlite3_int64 id_number = SQL_DNE;

    // Make sure the prepared statements are allocated.
    if(sql_get_english_word_to_id_code == NULL)
        return SQL_NOT_INIT; // Missing prepared statements

    if(sql_get_english_word_to_id_code != NULL) {
        sqlite3_bind_text(sql_get_english_word_to_id_code, 1, word, -1, NULL);

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

int sqlAddWord(const WordDefinition *const word_definition) {
    int db_return;

    // Make sure the prepared statements are allocated.
    if(sql_insert_dictionary_code == NULL || sql_get_entry_dictionary_id_code == NULL || sql_insert_english_translation_code == NULL)
        return SQL_NOT_INIT; // Missing prepared statements

    // Make sure that the word does not exist first.
    if(sqlGetWordIDLanguage(word_definition->word) != 0)
        return SQL_ONLY_ONE_ENTRY; // Language Word already exists.

    if(sqlGetWordIDEnglish(word_definition->keyword) != 0)
        return SQL_ONLY_ONE_ENTRY; // English Word already exists.

    {
        sqlite3_bind_text( sql_insert_dictionary_code, 1, word_definition->word, -1, NULL);
        sqlite3_bind_text( sql_insert_dictionary_code, 2, word_definition->parts_of_speech, -1, NULL);

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
        sqlite3_bind_text(sql_get_entry_dictionary_id_code, 1, word_definition->word, -1, NULL);
        sqlite3_bind_text(sql_get_entry_dictionary_id_code, 2, word_definition->parts_of_speech, -1, NULL);

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
        sqlite3_bind_text( sql_insert_english_translation_code, 2, word_definition->keyword, -1, NULL);
        sqlite3_bind_text( sql_insert_english_translation_code, 3, word_definition->definition, -1, NULL);

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

db_return_code sqlUpdateWord(int word_id, const WordDefinition *const word_definition) {
    int db_return;

    if(sql_update_dictionary_code == NULL || sql_update_english_translation_code == NULL)
        return SQL_NOT_INIT; // Missing prepared statements

    // Check if word_id exists.
    db_return_code status = sqlGetWord(word_id, NULL);

    if(status != SQL_SUCCESS)
        return status;

    int language_id = sqlGetWordIDLanguage(word_definition->word);

    {
        if(language_id != SQL_DNE && language_id != word_id)
            return SQL_ONLY_ONE_ENTRY;

        language_id = sqlGetWordIDEnglish(word_definition->keyword);

        if(language_id != SQL_DNE && language_id != word_id)
            return SQL_ONLY_ONE_ENTRY;
    }

    {
        sqlite3_bind_text( sql_update_dictionary_code, 1, word_definition->word, -1, NULL);
        sqlite3_bind_text( sql_update_dictionary_code, 2, word_definition->parts_of_speech, -1, NULL);
        sqlite3_bind_int64(sql_update_dictionary_code, 3, word_id);

        db_return = sqlite3_step(sql_update_dictionary_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_update_dictionary_code);
        }

        if(db_return != SQLITE_DONE) {
            printf("Sqlite3 prepare error: %s\n", sqlite3_errstr(db_return) );
        }

        sqlite3_reset(sql_update_dictionary_code);
    }

    {
        sqlite3_bind_text( sql_update_english_translation_code, 1, word_definition->keyword, -1, NULL);
        sqlite3_bind_text( sql_update_english_translation_code, 2, word_definition->definition, -1, NULL);
        sqlite3_bind_int64(sql_update_english_translation_code, 3, word_id);

        db_return = sqlite3_step(sql_update_english_translation_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_update_english_translation_code);
        }

        if(db_return != SQLITE_DONE) {
            printf("Sqlite3 prepare error: %s\n", sqlite3_errstr(db_return) );
        }

        sqlite3_reset(sql_update_english_translation_code);
    }

    return status;
}

db_return_code sqlGetWord(int word_id, WordDefinition *word_definition) {
    int db_return;
    db_return_code status = SQL_DNE;

    if(sql_get_dictionary_entry_code == NULL || sql_get_english_translation_entry_code == NULL)
        return SQL_NOT_INIT; // Missing prepared statements

    {
        sqlite3_bind_int64(sql_get_dictionary_entry_code, 1, word_id);

        db_return = sqlite3_step(sql_get_dictionary_entry_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_get_dictionary_entry_code);
        }

        if(db_return == SQLITE_ROW) {
            if(word_definition != NULL) {
                strncpy(word_definition->word, sqlite3_column_text(sql_get_dictionary_entry_code, 0), sizeof(word_definition->word) / sizeof(word_definition->word[0]));
                strncpy(word_definition->parts_of_speech, sqlite3_column_text(sql_get_dictionary_entry_code, 1), sizeof(word_definition->parts_of_speech) / sizeof(word_definition->parts_of_speech[0]));
            }

            db_return = sqlite3_step(sql_get_dictionary_entry_code);

            for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
                db_return = sqlite3_step(sql_get_dictionary_entry_code);
            }

            if(db_return != SQLITE_DONE) {
                printf("Sqlite3 preparey error: %s\n", sqlite3_errstr(db_return) );
            }
        }
        else if(db_return == SQLITE_DONE)
            status = SQL_DNE; // Redundent
        else
            printf("Sqlite3 prepare error: %s: %d\n", sqlite3_errstr(db_return), db_return );

        sqlite3_reset(sql_get_dictionary_entry_code);
    }

    {
        sqlite3_bind_int64(sql_get_english_translation_entry_code, 1, word_id);

        db_return = sqlite3_step(sql_get_english_translation_entry_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_get_english_translation_entry_code);
        }

        if(db_return == SQLITE_ROW) {
            if(word_definition != NULL) {
                strncpy(word_definition->keyword, sqlite3_column_text(sql_get_english_translation_entry_code, 0), sizeof(word_definition->keyword) / sizeof(word_definition->keyword[0]));
                strncpy(word_definition->definition, sqlite3_column_text(sql_get_english_translation_entry_code, 1), sizeof(word_definition->definition) / sizeof(word_definition->definition[0]));
            }

            db_return = sqlite3_step(sql_get_english_translation_entry_code);

            for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
                db_return = sqlite3_step(sql_get_english_translation_entry_code);
            }

            if(db_return != SQLITE_DONE) {
                printf("Sqlite3 preparey error: %s\n", sqlite3_errstr(db_return) );
            }
            else
                status = SQL_SUCCESS; // True
        }
        else if(db_return == SQLITE_DONE)
            status = SQL_DNE; // Redundent
        else
            printf("Sqlite3 prepare error: %s: %d\n", sqlite3_errstr(db_return), db_return );

        sqlite3_reset(sql_get_english_translation_entry_code);
    }

    return status;
}

db_return_code sqlRemoveWord(int word_id) {
    int db_return;

    if(sql_delete_dictionary_entry_code == NULL || sql_delete_english_translation_entry_code == NULL)
        return SQL_NOT_INIT; // Cannot delete without this prepared statement.

    db_return_code status = sqlGetWord(word_id, NULL);

    if(status != SQL_SUCCESS)
        return status;

    {
        sqlite3_bind_int64(sql_delete_dictionary_entry_code, 1, word_id);

        db_return = sqlite3_step(sql_delete_dictionary_entry_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_delete_dictionary_entry_code);
        }

        if(db_return != SQLITE_DONE) {
            printf("Sqlite3 prepare error: %s\n", sqlite3_errstr(db_return) );
        }

        sqlite3_reset(sql_delete_dictionary_entry_code);
    }

    {
        sqlite3_bind_int64(sql_delete_english_translation_entry_code, 1, word_id);

        db_return = sqlite3_step(sql_delete_english_translation_entry_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_delete_english_translation_entry_code);
        }

        if(db_return != SQLITE_DONE) {
            printf("Sqlite3 prepare error: %s\n", sqlite3_errstr(db_return) );
        }

        sqlite3_reset(sql_delete_english_translation_entry_code);
    }

    return status;
}

db_return_code sqlIsDatabaseOkay(char *destination, int destination_limit) {
    int db_return;
    sqlite3_int64 dictionary_count = 0;
    sqlite3_int64 english_translation_count = 0;

    // Make sure the prepared statements are allocated.
    if(sql_count_dictionary_entries_code == NULL || sql_count_english_translation_entries_code == NULL)
        return SQL_NOT_INIT; // Missing prepared statements

    {
        db_return = sqlite3_step(sql_count_dictionary_entries_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_count_dictionary_entries_code);
        }

        if(db_return == SQLITE_ROW)
            dictionary_count = sqlite3_column_int64(sql_count_dictionary_entries_code, 0);

        db_return = sqlite3_step(sql_count_dictionary_entries_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_count_dictionary_entries_code);
        }

        if(db_return != SQLITE_DONE) {
            printf("sqlGetWordIDLanguage prepare error: %s\n", sqlite3_errstr(db_return) );
        }

        sqlite3_reset(sql_count_dictionary_entries_code);
    }

    {
        db_return = sqlite3_step(sql_count_english_translation_entries_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_count_english_translation_entries_code);
        }

        if(db_return == SQLITE_ROW)
            english_translation_count = sqlite3_column_int64(sql_count_english_translation_entries_code, 0);

        db_return = sqlite3_step(sql_count_english_translation_entries_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_count_english_translation_entries_code);
        }

        if(db_return != SQLITE_DONE) {
            printf("sqlGetWordIDLanguage prepare error: %s\n", sqlite3_errstr(db_return) );
        }

        sqlite3_reset(sql_count_english_translation_entries_code);
    }

    if(dictionary_count != english_translation_count) {
        snprintf(destination, destination_limit, "Dictionary entries %d\nEnglish Translation entries %d\n", dictionary_count, english_translation_count);
        return SQL_INVALID_TABLE;
    }

    return SQL_SUCCESS;
}

void wordDefinitionStr(const WordDefinition *const def, char *destination, int destination_limit) {
    snprintf(destination, destination_limit, "Word: %s\nParts Of Speech: %s\nKeyword: %s\nDefinition: %s\n", def->word, def->parts_of_speech, def->keyword, def->definition);
}

static void sqlLitePrepare(const char *const statement, sqlite3_stmt **code) {
    int db_return;

    db_return = sqlite3_prepare_v2(database, statement, -1, code, NULL);
    if(db_return != SQLITE_OK)
        printf("Sqlite3 Statement: \"%s\"\nPrepare Error: %s\n\n", statement, sqlite3_errstr(db_return));
}

static int sqlLiteDoesTableExist(const char *const table_name) {
    int db_return;
    int found = 0; // False

    if(sql_check_for_table_code != NULL) {
        sqlite3_bind_text(sql_check_for_table_code, 1, table_name, -1, NULL);

        db_return = sqlite3_step(sql_check_for_table_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_check_for_table_code);
        }

        if(db_return == SQLITE_ROW)
            found = 1; // True

        db_return = sqlite3_step(sql_check_for_table_code);

        for(int limit = 0; limit < 256 && db_return == SQLITE_BUSY; limit++) {
            db_return = sqlite3_step(sql_check_for_table_code);
        }

        if(db_return != SQLITE_DONE) {
            printf("sqlLiteDoesTableExist prepare error: %s\n", sqlite3_errstr(db_return) );
        }

        sqlite3_reset(sql_check_for_table_code);
    }

    return found;
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
