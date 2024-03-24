#ifndef GRAMMER_DTLWS
#define GRAMMER_DTLWS

extern char lexer_status[0x100];
extern char grammer_status[0x100];

// implemented in grammer.y
int grammerParse(const char *string);

// implemented in lexicon.lex
int lexerParse(const char *string);

#endif // GRAMMER_DTLWS
