%{
#include <stdlib.h>
#include <stdio.h>

// Declared in lex.yy.c
// https://stackoverflow.com/questions/780676/string-input-to-flex-lexer
// Thank you sevko
typedef struct yy_buffer_state* YY_BUFFER_STATE;
YY_BUFFER_STATE yy_scan_string(const char * str);
void yy_delete_buffer(YY_BUFFER_STATE buffer);

extern int lex_line;
%}

%start choose

%union {
     char word[128];
     int number;
}

%token GROUP_BEGIN GROUP_END END
%token IF ELIF ELSE THEN

%token <word> DELIMITER CONJUNCTION PROPOSITION

%token <word> PRONOUN NOUN ADJECTIVE VERB ADVERB

%token <number> NUMBER_PLACE NUMBER_100 NUMBER_10 NUMBER_1

%%
choose:
     IF if_group choose
    |noun_phrase verb_phrase end_phrase DELIMITER choose
    |END;
if_group:
     GROUP_BEGIN condition_words GROUP_END then elif;
then:
     THEN GROUP_BEGIN words GROUP_END;
elif:
     ELIF GROUP_BEGIN condition_words GROUP_END then elif
    |ELSE then
    |;
condition_words:
     sentence CONJUNCTION condition_words
    |sentence DELIMITER;
words:
     sentence CONJUNCTION words
    |sentence DELIMITER words
    |IF if_group words
    |;
sentence:
     noun_phrase verb_phrase end_phrase;
verb_phrase:
     VERB adverbs;
adverbs:
     adverbs ADVERB
    |;
adjectives:
     adjectives ADJECTIVE
    |;
end_phrase:
    noun_phrase
    |;
noun_phrase:
     noun adjectives number preposition_phrase;
preposition_phrase:
     PROPOSITION noun adjectives number
    |;
noun:
     NOUN | PRONOUN;
number:
     NUMBER_100 number_10 number
    |NUMBER_10 number_1 number
    |NUMBER_1 NUMBER_PLACE number
    |;
number_10:
     NUMBER_10 number_1
    |number_1;
number_1:
     NUMBER_1 NUMBER_PLACE
    |NUMBER_PLACE;
%%

int yyerror(char *why) {
     printf("Parsing Error!: %s at line %d\n", why, lex_line);
     exit(0);
}


int grammerParse(const char *string) {
     YY_BUFFER_STATE buffer_state = yy_scan_string ( string );

     int yy_result = yyparse();

     yy_delete_buffer(buffer_state);
}

int main() {
     // return grammerParse("if g_begin caro reda manoa ba 10s thousand of seno na ba noa 1s thousand isas veno ia ria 10s 1s thousand of manos losta dama isasa 100s 1s thousand and mio isas goldo end g_end then g_begin mano itas bono end if g_begin mio isas galdo end g_end then g_begin mio isas note rico end g_end g_end mario isas greeno of legondo 1s thousand end END");
     return yyparse();
}
