#ifndef SQL_DTLWS
#define SQL_DTLWS

typedef struct {
    char word[128];
    char parts_of_speech[64];
    char keyword[64];
    char definition[768];
} WordDefinition;

int sqlInit(const char *const path);
void sqlDeinit();

int sqlGetWordIDLanguage(const char *const word);
int sqlGetWordIDEnglish(const char *const word);
int sqlAddWord(const WordDefinition *const word_definition);
int sqlUpdateWord(int word_id, const WordDefinition *const word_definition);

#endif // SQL_DTLWS
