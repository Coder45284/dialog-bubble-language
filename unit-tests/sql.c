#include "sql.h"

int main() {

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

     return 0;
}
