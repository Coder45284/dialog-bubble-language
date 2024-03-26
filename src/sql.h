#ifndef SQL_DTLWS
#define SQL_DTLWS

typedef struct {
    char word[128];
    char parts_of_speech[64];
    char keyword[64];
    char definition[768];
} WordDefinition;

// Does not exist.
#define SQL_SUCCESS 1
#define SQL_DNE 0
#define SQL_NOT_INIT -1
#define SQL_ONLY_ONE_ENTRY -2

int sqlInit(const char *const path);
void sqlDeinit();

int sqlGetWordIDLanguage(const char *const word);
int sqlGetWordIDEnglish(const char *const word);
int sqlAddWord(const WordDefinition *const word_definition);
int sqlUpdateWord(int word_id, const WordDefinition *const word_definition);
int sqlGetWord(int word_id, WordDefinition *word_definition);
int sqlRemoveWord(int word_id);

#endif // SQL_DTLWS
