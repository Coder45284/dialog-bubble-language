#ifndef GRAMMER_DTLWS
#define GRAMMER_DTLWS

#include "y.tab.h"

extern char lexer_status[0x100];
extern char grammer_status[0x100];

// implemented in grammer.y
int grammerParse(const char *string);

// implemented in lexicon.lex
int lexerParse(const char *string);
void lexerNullCallback(int token_type, const YYSTYPE *const yystype);
extern void (*lex_callback)(int token_type, const YYSTYPE *const yystype);

#endif // GRAMMER_DTLWS
