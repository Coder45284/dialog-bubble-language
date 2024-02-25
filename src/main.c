#include <raylib.h>
#include <stdio.h>
#include <math.h>

#include "constants.h"

typedef struct {
    int  on_period;
    int off_period;
    int period;
    PCM_SAMPLE_TYPE amplitude;
} Note;

#define DEFINE_NOTE( name, times_a_second, time_off, amp, frequency) \
Note name = {(float)PCM_SAMPLES_PER_SECOND / ((float)times_a_second), ((float)PCM_SAMPLES_PER_SECOND / (float)times_a_second) * (time_off), (float)PCM_SAMPLES_PER_SECOND / (frequency), amp}

typedef struct {
    const Note *const note_r;
    int time;
} Context;

DEFINE_NOTE(note, 8.0, 0.25, 2048, 2000);
Context context = {&note, 0};

void soundCallback(void *buffer_data, unsigned int frames) {
    PCM_SAMPLE_TYPE *frame_data = (PCM_SAMPLE_TYPE*)buffer_data;

    int true_off_period = context.note_r->off_period / context.note_r->period;
    true_off_period += (context.note_r->off_period % context.note_r->period) != 0;
    true_off_period *= context.note_r->period;

    for( unsigned int f = 0; f < frames; f++ ) {
        if(context.time % context.note_r->period > (context.note_r->period / 2)) {
            frame_data[f] =  context.note_r->amplitude;
        }
        else {
            frame_data[f] = -context.note_r->amplitude;
        }
        frame_data[f] = (double)context.note_r->amplitude * sin(2. * M_PI * (context.time % context.note_r->period) / (double)context.note_r->period);

        if(context.time > true_off_period)
            frame_data[f] = 0;

        context.time++;

        if(context.time >= context.note_r->on_period)
            context.time = 0;
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
