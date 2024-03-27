#ifndef SQL_DTLWS
#define SQL_DTLWS

typedef struct {
    char word[128];
    char parts_of_speech[64];
    char keyword[64];
    char definition[768];
} WordDefinition;

typedef enum {
    SQL_SUCCESS = 1,
    SQL_DNE = 0, // This means does not exist.
    SQL_NOT_INIT = -1,
    SQL_ONLY_ONE_ENTRY = -2
} db_return_code;

int sqlInit(const char *const path);
void sqlDeinit();

int sqlGetWordIDLanguage(const char *const word);
int sqlGetWordIDEnglish(const char *const word);
int sqlAddWord(const WordDefinition *const word_definition);

db_return_code sqlUpdateWord(int word_id, const WordDefinition *const word_definition);
db_return_code sqlGetWord(int word_id, WordDefinition *word_definition);
db_return_code sqlRemoveWord(int word_id);

void wordDefinitionStr(const WordDefinition *const word_definition, char *destination, int destination_limit);

#endif // SQL_DTLWS
