#ifndef DATA_BASE_DTLWS
#define DATA_BASE_DTLWS

typedef struct {
    char word[128];
    char parts_of_speech[64];
    char keyword[64];
    char definition[768];
} DBWordDefinition;

typedef enum {
    DB_SUCCESS = 1,
    DB_DNE = 0, // This means does not exist.
    DB_NOT_INIT = -1,
    DB_ONLY_ONE_ENTRY = -2,
    DB_INVALID_TABLE = -3
} db_return_code;

int dbInit(const char *const path);
void dbDeinit();

int dbGetWordIDLanguage(const char *const word);
int dbGetWordIDEnglish(const char *const word);
int dbAddWord(const DBWordDefinition *const word_definition);

db_return_code dbUpdateWord(int word_id, const DBWordDefinition *const word_definition);
db_return_code dbGetWord(int word_id, DBWordDefinition *word_definition);
db_return_code dbRemoveWord(int word_id);

db_return_code dbIsDatabaseOkay(char *destination, int destination_limit);

void dbWordDefinitionStr(const DBWordDefinition *const word_definition, char *destination, int destination_limit);

#endif // DATA_BASE_DTLWS
