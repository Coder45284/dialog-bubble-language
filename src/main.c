#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "constants.h"
#include "voice.h"

int main() {
    // generateAllPhonemics();

    context.note_amount = 0;

    const char person[] = {'s','q','t','w'};
    const char gender[] = {'s','q', '\0'};
    const char  owner[] = {'s','t'};
    const char plural[] = {'e','h'};
    char single_phonem[4] = {'\0', '\0', '\0', '\0'};

    for(int p = 0; p < 4; p++) {
        for(int g = 0; g < 3; g++) {
            for(int o = 0; o < 2; o++) {
                for(int r = 0; r < 2; r++) {
                    // 's' is 1st person, 'q' is 2nd person, 't' is 3rd person for people, 'w' is 3rd person for objects
                    single_phonem[0] = person[p];
                    single_phonem[1] = 'i';
                    single_phonem[2] = 'h';
                    inputPhonemic(single_phonem);

                    if(gender[g] != '\0') {
                        single_phonem[0] = gender[g];
                        single_phonem[1] = 'e';
                        single_phonem[2] = 'e';
                        inputPhonemic(single_phonem);
                    }

                    single_phonem[0] = owner[o];
                    single_phonem[1] = 'o';
                    single_phonem[2] = plural[r];
                    inputPhonemic(single_phonem);

                    // Insert space
                    inputPhonemic("");

                    if(gender[g] == '\0')
                        inputPhonemic("");
                }
            }
        }
    }

    setupContext();

    exportWAV("All Pronouns.wav");

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dialog Text Language Work Station");

    InitAudioDevice();
    SetAudioStreamBufferSizeDefault(VOICE_BUFFER_SIZE);

    AudioStream voice = LoadAudioStream(PCM_SAMPLES_PER_SECOND, PCM_SAMPLE_BITS, 1);

    SetAudioStreamCallback(voice, soundCallback);

    PlayAudioStream(voice);

    SetTargetFPS(30);

    while(!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(WHITE);

        EndDrawing();
    }

    UnloadAudioStream(voice);
    CloseAudioDevice();

    CloseWindow();

    return 0;
}
