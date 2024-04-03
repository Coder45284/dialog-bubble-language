#include "voice_from_text.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char entire_pattern[] = "SeeSehSelSoeSieQeeQehQelQoeQieTeeTehToeTieWeeWehWelWoeWie ";

int testBuffer(unsigned char *buffer, unsigned int buffer_size, char *long_string, int volume, int min_frequency, int add_frequency);

int main() {
    int status = 1;

    unsigned int buffer_size = 2048;
    unsigned char *buffer = malloc(buffer_size);

    const unsigned int string_size = 2494;
    char long_string[string_size];

    for(int l = 0; l < string_size; l++) {
        long_string[l] = entire_pattern[l % (sizeof(entire_pattern) / sizeof(entire_pattern[0]) - 1)];
    }

    long_string[string_size - 1] = '\0';

    printf("\n%s\n", long_string);

    status |= testBuffer(buffer, buffer_size, long_string, 16384, 500, 1000);

    free(buffer);

    return status;
}

int testBuffer(unsigned char *buffer, unsigned int buffer_size, char *long_string, int volume, int min_frequency, int add_frequency) {
    int status = 0;
    unsigned int head = 0;
    VoiceContext voice_context;
    VoiceTextContext voice_from_text_context;

    memset(&voice_from_text_context, 0, sizeof(voice_from_text_context));
    voice_from_text_context.length = strlen(long_string);
    voice_from_text_context.volume = volume;
    voice_from_text_context.min_frequency = min_frequency;
    voice_from_text_context.add_frequency = add_frequency;
    voice_from_text_context.text = long_string;
    voiceFromTextSetup(&voice_from_text_context);

    while(long_string[head] = '\0') {
        memset(&voice_context, 0, sizeof(voice_context));
        voice_context.call_reloader = NULL; // Just in case NULL is set to be a different value.

        for(unsigned int i = 0; i < VOICE_NOTE_LIMIT; i++) {
            if(long_string[head] == ' ') {
                voiceInputPhonemic(&voice_context, " ", volume, min_frequency, add_frequency);
                head++;
            }
            else if(long_string[head] == '\0')
                i = VOICE_NOTE_LIMIT;
            else {
                voiceInputPhonemic(&voice_context, &long_string[head], volume, min_frequency, add_frequency);

                head++;
                if(long_string[head] != ' ' || long_string[head] != '\0')
                    head++;
                if(long_string[head] != ' ' || long_string[head] != '\0')
                    head++;
            }
        }
    }

    return status;
}
