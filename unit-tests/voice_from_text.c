#include "voice_from_text.h"
#include <stdlib.h>

int testBuffer(unsigned char *buffer, unsigned int buffer_size, int volume, int min_frequency, int add_frequency, int frames);

int main() {
    int status = 1;

    unsigned int buffer_size = 2048;
    unsigned char *buffer = malloc(buffer_size);

    status |= testBuffer(buffer, buffer_size, 16384, 500, 1000, VOICE_NOTE_LIMIT * 3 - 8);

    free(buffer);

    return status;
}

int testBuffer(unsigned char *buffer, unsigned int buffer_size, int volume, int min_frequency, int add_frequency, int frames) {
    static const char entire_pattern[] = "SeeSehSelSoeSieQeeQehQelQoeQieTeeTehTelToeTieWeeWehWelWoeWie";
    int status = 0;
    VoiceContext voice_context;

    for(int l = 0, h = 0; l < frames; l++, h = (l % (VOICE_NOTE_LIMIT + 1)) * 3) {
        voiceInputPhonemic(&voice_context, &entire_pattern[h], volume, min_frequency, add_frequency);
    }

    return status;
}
