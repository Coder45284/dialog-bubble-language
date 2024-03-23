#ifndef GRAMMER_DTLWS
#define GRAMMER_DTLWS

extern char lexer_status[0x100];
extern char grammer_status[0x100];

// implemented in grammer.y
void grammerParse(const char *string);

// implemented in lexicon.lex
void lexerParse(const char *string);

#endif // GRAMMER_DTLWS
