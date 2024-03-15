#ifndef GRAMMER_DTLWS
#define GRAMMER_DTLWS

extern char lexer_status[0x100];
extern char grammer_status[0x100];

// implemented in grammer.y
void grammerParse();

#endif // GRAMMER_DTLWS
