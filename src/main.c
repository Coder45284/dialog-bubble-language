#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "constants.h"
#include "voice.h"

int main() {
    // generateAllPhonemics();

    context.note_amount = 0;

    // This is saying 'we'
    inputPhonemic("sih"); // 1st person
    // inputPhonemic("see"); // Optional gender marker.
    inputPhonemic("soh"); // plural
    inputPhonemic("");

    setupContext();

    // exportWAV("new.wav");

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
