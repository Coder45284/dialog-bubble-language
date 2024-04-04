#include "grammer.h"

#include <stdio.h>

void lexerTestCallback(int token_type, const YYSTYPE *const yystype);

int main() {
    static const char text[] = "Tie Sie TieSoeToe QeeQeeQie QeeTeeQie QelWelQie TeeWelTee SeeTeh SeeSee SeeQehSie WoeTehQie TeeWeeQie TeeWeeWeeQie SeeWehTee QeeSeeTeh WehSehTee TelQelTee WeeQeeTee QeeTeh WehTee SelTee WeeTeeTee SeeTeh QehSehTee TelSelTee WeeSeeTee Qel QieWelSoe TehQeeWee WeeQeeSie WoeTehQie QeeTeeQie SeeWehTee SeeTeh TelQelSelTee WeeTeeQeeSeeTee SeeSee QelQehSie TeeWeeWeeQie WoeTehQie QeeQeeQie TeeWelTee SeeTeh WehTehTee QeeTee SeeTel WehTehQehSehTee WelTelQelSelTee WeeTeeQeeSeeTee Seh SieSoe TehQeeWee SeeSeeSehSie See Soe Toe Sie QelQehSie SeeSeeSeeQeeWel QelQehSie See Tie Sie SieSoe TehQeeWee TeeQeeSieToe See Soe Toe Sie SieSoe TehQeeWee QeeToeWie TeeTeeSieToe See Soe Soe TeeWeeSie TehQeeWee QeeTeeTeeSie SeeSee QehQehTieSieToe SeeWehTee SeeTeh TeeSeeTee Qee WehSeeQel TehQeeWee TieWelSoe Qel QieWelSoe Tel WehSeeQolTeh TehQeeWee TieWelSoe See WehSeeTel TehQeeWee TieWelSoe See WehSeeWeh TehQeeWee TieWelSoe Sel WehSeeWoe TehQeeWee QeeTelQee TieWelSoe See";

    int status = 1;

    lex_callback = lexerTestCallback;

    lexerParse(text);

    return status;
}

void lexerTestCallback(int token_type, const YYSTYPE *const yystype) {
    printf("lexerTestCallback %d = %s\n", token_type, yystype->word);
}
