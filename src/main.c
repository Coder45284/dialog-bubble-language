#include <raylib.h>
#include <stdio.h>

#include "constants.h"

typedef struct {
    unsigned int  on_period;
    unsigned int off_period;
    PCM_SAMPLE_TYPE amplitude;

} Note;

typedef struct {
    const Note *const note_r;
    unsigned int period;
} Context;

Note note = {(float)PCM_SAMPLES_PER_SECOND / 4.0, (float)PCM_SAMPLES_PER_SECOND / 4.0 - (float)PCM_SAMPLES_PER_SECOND / 8.0, 2048};
Context context = {&note, 0};

void soundCallback(void *buffer_data, unsigned int frames) {
    PCM_SAMPLE_TYPE *frame_data = (PCM_SAMPLE_TYPE*)buffer_data;

    for( unsigned int f = 0; f < frames; f++ ) {
        if(context.period % 256 > 128) {
            frame_data[f] =  context.note_r->amplitude;
        }
        else
            frame_data[f] = -context.note_r->amplitude;

        context.period++;

        if(context.period >= 256)
            context.period = 0;
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
