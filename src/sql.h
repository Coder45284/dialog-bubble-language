#ifndef SQL_DTLWS
#define SQL_DTLWS

int sqlInit();
void sqlDeinit();

int sqlGetWordIDLanguage(const char *const word);
int sqlGetWordIDEnglish(const char *const word);
int sqlAddWord(const char *const word, const char *const parts_of_speech, const char *const english_keyword, const char *const english_definition);
int sqlUpdateWord(int word_id, const char *const word, const char *const parts_of_speech, const char *const english_keyword, const char *const english_definition);

#endif // SQL_DTLWS
