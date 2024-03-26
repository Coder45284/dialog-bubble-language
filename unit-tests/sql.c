#include "sql.h"

#include <stdio.h>

int main() {
    int problem = 0;

    if(sqlGetWordIDLanguage("QeeSoeWee") != SQL_NOT_INIT) {
        problem |= 1;
        printf("SQL_NOT_INIT sqlGetWordIDLanguage\n");
    }

    if(sqlGetWordIDEnglish("RUN") != SQL_NOT_INIT) {
        problem |= 1;
        printf("SQL_NOT_INIT sqlGetWordIDEnglish\n");
    }
    {
        WordDefinition word_def = {"QeeSoeWee", "VERB", "RUN", "The action of running."};

        if(sqlAddWord(&word_def) != SQL_NOT_INIT) {
            problem |= 1;
            printf("SQL_NOT_INIT sqlAddWord\n");
        }

        if(sqlUpdateWord(1, &word_def) != SQL_NOT_INIT) {
            problem |= 1;
            printf("SQL_NOT_INIT sqlUpdateWord\n");
        }

        if(sqlGetWord(1, &word_def) != SQL_NOT_INIT) {
            problem |= 1;
            printf("SQL_NOT_INIT sqlGetWord\n");
        }

        if(sqlRemoveWord(1) != SQL_NOT_INIT) {
            problem |= 1;
            printf("SQL_NOT_INITsqlRemoveWord\n");
        }
    }


    {
        WordDefinition word_def = {"QeeSoeWee", "VERB", "RUN", "The action of running."};
        sqlAddWord(&word_def);
    }
    {
        WordDefinition word_def = {"QeeSeeWel", "NOUN;ADJECTIVE", "RED", "The color red."};
        sqlAddWord(&word_def);
    }
    {
        WordDefinition word_def = {"QoeQoeWeh", "NOUN", "CART", "A cart like a shopping cart."};
        sqlAddWord(&word_def);
    }
    {
        WordDefinition word_def = {"ToeQeeWee", "ADVERB;ADJECTIVE", "SLOW", "Slow movement or to describe something with slow movement."};
        sqlAddWord(&word_def);
    }
    {
        WordDefinition word_def = {"Weh", "PARTITION", "CARD", "It is non sense."};
        sqlUpdateWord(sqlGetWordIDEnglish("CART"), &word_def);
    }

     return problem;
}
