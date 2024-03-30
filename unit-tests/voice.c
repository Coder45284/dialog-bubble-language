#include "voice.h"

#include "audio_1.h"
#include "audio_2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_buffer(unsigned char *buffer, unsigned int buffer_size);

int main() {
    if(AUDIO_1_FRAME_COUNT != AUDIO_2_FRAME_COUNT) {
        printf("AUDIO_1_FRAME_COUNT != AUDIO_2_FRAME_COUNT.\n"
            "AUDIO_1_FRAME_COUNT = %d.\n"
            "AUDIO_2_FRAME_COUNT = %d.\n", AUDIO_1_FRAME_COUNT, AUDIO_2_FRAME_COUNT);
        return 1;
    }
    if(AUDIO_1_SAMPLE_RATE != AUDIO_2_SAMPLE_RATE) {
        printf("AUDIO_1_SAMPLE_RATE != AUDIO_2_SAMPLE_RATE.\n"
            "AUDIO_1_SAMPLE_RATE = %d.\n"
            "AUDIO_2_SAMPLE_RATE = %d.\n", AUDIO_1_SAMPLE_RATE, AUDIO_2_SAMPLE_RATE);
        return 1;
    }
    if(AUDIO_1_SAMPLE_SIZE != AUDIO_2_SAMPLE_SIZE) {
        printf("AUDIO_1_SAMPLE_SIZE != AUDIO_2_SAMPLE_SIZE.\n"
            "AUDIO_1_SAMPLE_SIZE = %d.\n"
            "AUDIO_1_SAMPLE_SIZE = %d.\n", AUDIO_1_SAMPLE_SIZE, AUDIO_2_SAMPLE_SIZE);
        return 1;
    }
    if(AUDIO_1_CHANNELS != AUDIO_2_CHANNELS) {
        printf("AUDIO_1_CHANNELS != AUDIO_2_CHANNELS.\n"
            "AUDIO_1_CHANNELS = %d.\n"
            "AUDIO_1_CHANNELS = %d.\n", AUDIO_1_CHANNELS, AUDIO_2_CHANNELS);
        return 1;
    }

    // Check the endianess of the CPU.
    char bytes[4] = {0x12, 0x34, 0x56, 0x78};
    short word_check = *((short*)bytes);

    if(word_check != 0x3412) {
        printf("This computer architecture might not be little endian!\nThis test was designed only for little endian with 16 bit shorts.\n");
        return 1;
    }

    unsigned int buffer_size = sizeof(AUDIO_1_DATA);
    unsigned char *buffer = malloc(buffer_size);

    int result = test_buffer(buffer, buffer_size);

    free(buffer);

    return result;
}

int test_buffer(unsigned char *buffer, unsigned int buffer_size) {
    static const int volume = 16384;
    static const int min_frequency[2] = {500, 250};
    static const int add_frequency[2] = {1000, 500};
    static const unsigned char* audios[2] = {AUDIO_1_DATA, AUDIO_2_DATA};

    static const char entire_alphabet[] = "SeeSehSelSoeSieQeeQehQelQoeQie TeeTehTelToeTieWeeWehWelWoeWie";

    VoiceContext current_voice_context;
    VoiceContext voice_context[2];

    // Setup the voice_contexts

    for(unsigned int slice = AUDIO_1_FRAME_COUNT; slice != 0; ) {
        unsigned int frame_amount = AUDIO_1_FRAME_COUNT / slice;
        unsigned int last_frame = AUDIO_1_FRAME_COUNT % slice;

        for(int i = 0; i < 2; i++) {
            memset(&current_voice_context, 0, sizeof(current_voice_context));
            current_voice_context.call_reloader = NULL; // Just in case NULL is set to be a different value.

            for(int h = 0; h < 5; h++) {
                static const char start[5] = {'S','Q', ' ', 'T', 'W'};

                if(start[h] == ' ') {
                    voiceInputPhonemic(&current_voice_context, "", volume, min_frequency[i], add_frequency[i]);
                    continue;
                }

                for(int g = 0; g < 5; g++) {
                    static const char last[5][2] = {{'e', 'e'}, {'e', 'h'}, {'e', 'l'}, {'o', 'e'}, {'i', 'e'}};

                    const char word[] = {start[h], last[g][0], last[g][1], '\0'};

                    voiceInputPhonemic(&current_voice_context, word, volume, min_frequency[i], add_frequency[i]);
                }
            }
            voiceInputPhonemic(&current_voice_context, "", volume, min_frequency[0], add_frequency[i]);
            voiceReadyContext(&current_voice_context);

            memset(&voice_context[i], 0, sizeof(voice_context[i]));
            voice_context[i].call_reloader = NULL; // Just in case NULL is set to be a different value.

            voiceInputPhonemics(&voice_context[i], entire_alphabet, sizeof(entire_alphabet) / sizeof(entire_alphabet[0]), volume, min_frequency[i], add_frequency[i]);

            int mem_data = memcmp(&current_voice_context, &voice_context[i], sizeof(voice_context[i]));

            if(mem_data != 0) {
                printf("Error: current_voice_context is not the same as voice_context[%d]; %d\n", i, mem_data);
                return 1;
            }

            memset(buffer, 0x1e, sizeof(AUDIO_1_DATA));

            for(unsigned int f = 0; f < frame_amount; f++) {
                voiceWriteToSoundBuffer(&current_voice_context, &buffer[sizeof(short) * f * slice], slice);
            }

            if(last_frame != 0) {
                voiceWriteToSoundBuffer(&current_voice_context, &buffer[buffer_size - sizeof(short) * last_frame], last_frame);
            }

            for(unsigned int b = 0; b < sizeof(AUDIO_1_DATA); b++) {
                if(audios[i][b] != buffer[b]) {
                    printf("Error audio did not produce correct results.\n"
                        "  min_frequency = %d; add_frequency = %d; test_index = %d\n", min_frequency[i], add_frequency[i], i);
                    printf("  s = %d; a = %d; f = %d; s * a + f = %d\n", slice, frame_amount, last_frame, slice * frame_amount + last_frame);
                    printf("  byte offset = %d; expected %d. received %d.\n", b, audios[i][b], buffer[b]);
                    return 1;
                }
            }
        }

        // With this this whole loop would go for only 20 times rather than 115752 times.
        if(slice > 10) // Divide up the sound generation beyond the size of 10.
        {
            slice /= 2;

            if(slice < 10)
                slice = 10;
        }
        else
            slice--; // less than 10. Just subtract by one.
    }

    return 0;
}
