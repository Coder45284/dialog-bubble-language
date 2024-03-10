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
     long long int number;
}

%token GROUP_BEGIN GROUP_END END
%token IF ELIF ELSE THEN

%token <word> DELIMITER CONJUNCTION PROPOSITION

%token <word> PRONOUN NOUN ADJECTIVE VERB ADVERB

%token <number> NUMBER_SIGN NUMBER_PLACE NUMBER_100 NUMBER_10 NUMBER_1

%type <number> number_place number_100 number_10 number_1

%%
choose:
     IF if_group choose
    |noun_phrase verb_phrase end_phrase DELIMITER choose
    |END { printf("Grammer has passed without problems.\n"); };
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
     NUMBER_SIGN number_place {
          long long int number = $1 * ($2);
          char space = ' ';

          if( number < 0 ) {
               space = '-';
               number = -number;
          }

          printf("Adjective Number (hex): %c%lx\n", space, number);
     }
    |;
number_place:
     NUMBER_PLACE number_100 number_place {$$ = (($2) << ($1 * 12)) + ($3);}
    | {$$ = 0;};
number_100:
     NUMBER_100 number_10 {$$ = ((($1) << 8) + $2);}
    |number_10 {$$ = $1;};
number_10:
     NUMBER_10 number_1 {$$ = ((($1) << 4) + $2);}
    |number_1 {$$ = $1;};
number_1:
     NUMBER_1 {$$ = $1;}
    | {$$ = 0;};
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
     // return grammerParse("if g_begin caro reda manoa ba 10s 1000s of seno na ba noa 1s 1000s isas veno ia ria 10s 1s 1000s of manos losta dama isasa 100s 1s 1000s and mio isas goldo end g_end then g_begin mano itas bono end if g_begin mio isas galdo end g_end then g_begin mio isas note rico end g_end g_end mario isas greeno of legondo 1s 1000s end END");
     return yyparse();
}
