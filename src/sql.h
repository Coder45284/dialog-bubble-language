#ifndef SQL_DTLWS
#define SQL_DTLWS

int sqlInit();
void sqlDeinit();

int sqlAddWord(const char *const word, const char *const parts_of_speech, const char *const english_keyword, const char *const english_definition);

#endif // SQL_DTLWS
