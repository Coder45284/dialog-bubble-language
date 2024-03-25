#include "sql.h"

int main() {

    sqlAddWord("QeeSoeWee", "VERB",              "RUN", "The action of running.");
    sqlAddWord("QeeSeeWel", "NOUN;ADJECTIVE",    "RED", "The color red.");
    sqlAddWord("QoeQoeWeh", "NOUN",             "CART", "A cart like a shopping cart.");
    sqlAddWord("ToeQeeWee", "ADVERB;ADJECTIVE", "SLOW", "Slow movement or to describe something with slow movement.");
    sqlUpdateWord(sqlGetWordIDEnglish("CART"),   "Weh", "PARTITION", "CARD", "It is non sense.");

     return 0;
}
