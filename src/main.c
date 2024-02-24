#include <raylib.h>
#include <stdio.h>

#include "constants.h"

PCM_SAMPLE_TYPE max_amplitude = 512 * 4;

void soundCallback(void *bufferData, unsigned int frames) {
    PCM_SAMPLE_TYPE *frameData = (PCM_SAMPLE_TYPE*)bufferData;

    for( unsigned int f = 0; f < frames; f++ ) {
        if( f % 256 > 128 ) {
            frameData[f] = max_amplitude;
        }
        else
            frameData[f] = -max_amplitude;
    }
}

int main() {
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
