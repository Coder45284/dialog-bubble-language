#ifndef SQL_DTLWS
#define SQL_DTLWS

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
int sqlAddWord(const DBWordDefinition *const word_definition);

db_return_code sqlUpdateWord(int word_id, const DBWordDefinition *const word_definition);
db_return_code sqlGetWord(int word_id, DBWordDefinition *word_definition);
db_return_code sqlRemoveWord(int word_id);

db_return_code sqlIsDatabaseOkay(char *destination, int destination_limit);

void wordDefinitionStr(const DBWordDefinition *const word_definition, char *destination, int destination_limit);

#endif // SQL_DTLWS
