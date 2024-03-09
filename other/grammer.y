%{
#include <stdlib.h>
#include <stdio.h>
%}

%start choose

%union {
     char word[128];
     int number;
}

%token GROUP_BEGIN GROUP_END
%token IF ELIF ELSE THEN

%token <word> DELIMITER CONJUNCTION PROPOSITION

%token <word> PRONOUN NOUN ADJECTIVE VERB ADVERB

%token <number> NUMBER_PLACE NUMBER_100 NUMBER_10 NUMBER_1

%%
choose:
     IF if_group choose
    |noun_phrase verb_phrase end_phrase DELIMITER choose
    |;
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
    printf("Parsing Error!: %s\n", why);
    exit(0);
}

int main() {
    return yyparse();
}
