#include "voice_from_text.h"
#include <stdlib.h>

int testBuffer(unsigned char *buffer, unsigned int buffer_size, int volume, int min_frequency, int add_frequency, int frames);

int main() {
    int status = 1;

    unsigned int buffer_size = 2048;
    unsigned char *buffer = malloc(buffer_size);

    static const char rambling[] = "Tie Sie TieSoeToe QeeQeeQie QeeTeeQie QelWelQie TeeWelTee SeeTeh SeeSee SeeQehSie WoeTehQie TeeWeeQie TeeWeeWeeQie SeeWehTee QeeSeeTeh WehSehTee TelQelTee WeeQeeTee QeeTeh WehTee SelTee WeeTeeTee SeeTeh QehSehTee TelSelTee WeeSeeTee Qel QieWelSoe TehQeeWee WeeQeeSie WoeTehQie QeeTeeQie SeeWehTee SeeTeh TelQelSelTee WeeTeeQeeSeeTee SeeSee QelQehSie TeeWeeWeeQie WoeTehQie QeeQeeQie TeeWelTee SeeTeh WehTehTee QeeTee SeeTel WehTehQehSehTee WelTelQelSelTee WeeTeeQeeSeeTee Seh SieSoe TehQeeWee SeeSeeSehSie See Soe Toe Sie QelQehSie SeeSeeSeeQeeWel QelQehSie See Tie Sie SieSoe TehQeeWee TeeQeeSieToe See Soe Toe Sie SieSoe TehQeeWee QeeToeWie TeeTeeSieToe See Soe Soe TeeWeeSie TehQeeWee QeeTeeTeeSie SeeSee QehQehTieSieToe SeeWehTee SeeTeh TeeSeeTee Qee WehSeeQel TehQeeWee TieWelSoe Qel QieWelSoe Tel WehSeeQolTeh TehQeeWee TieWelSoe See WehSeeTel TehQeeWee TieWelSoe See WehSeeWeh TehQeeWee TieWelSoe Sel WehSeeWoe TehQeeWee QeeTelQee TieWelSoe See";

    status |= testBuffer(buffer, buffer_size, 16384, 500, 1000, VOICE_NOTE_LIMIT * 3 - 8);

    free(buffer);

    return status;
}

int testBuffer(unsigned char *buffer, unsigned int buffer_size, int volume, int min_frequency, int add_frequency, int frames) {
    static const char entire_pattern[] = "SeeSehSelSoeSieQeeQehQelQoeQieTeeTehTelToeTieWeeWehWelWoeWie";
    int status = 0;
    VoiceContext voice_context;

    for(int l = 0, h = 0; l < frames; l++, h = (l % (VOICE_NOTE_LIMIT + 1)) * 3) {
        if(h != VOICE_NOTE_LIMIT * 3)
            voiceInputPhonemic(&voice_context, &entire_pattern[h], volume, min_frequency, add_frequency);
        else
            voiceInputPhonemic(&voice_context, " ", volume, min_frequency, add_frequency);
    }



    return status;
}
