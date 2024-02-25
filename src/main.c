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
const Note name = {(float)PCM_SAMPLES_PER_SECOND / ((float)times_a_second), ((float)PCM_SAMPLES_PER_SECOND / (float)times_a_second) * (time_off), (float)PCM_SAMPLES_PER_SECOND / (frequency), amp}

typedef struct {
    Note notes[32];
    unsigned int note_index;
    int time;
} Context;

DEFINE_NOTE(n200, 8.0, 0.25, 2048, 200);
DEFINE_NOTE(n400, 8.0, 0.25, 2048, 400);
DEFINE_NOTE(n600, 8.0, 0.25, 2048, 600);
DEFINE_NOTE(n800, 8.0, 0.25, 2048, 800);
DEFINE_NOTE(n1000, 8.0, 0.25, 2048, 1000);
DEFINE_NOTE(n1200, 8.0, 0.25, 2048, 1200);
DEFINE_NOTE(n1400, 8.0, 0.25, 2048, 1400);
DEFINE_NOTE(n1600, 8.0, 0.25, 2048, 1600);
DEFINE_NOTE(n1800, 8.0, 0.25, 2048, 1800);
DEFINE_NOTE(n2000, 8.0, 0.25, 2048, 2000);
DEFINE_NOTE(n2200, 8.0, 0.25, 2048, 2200);
DEFINE_NOTE(n2400, 8.0, 0.25, 2048, 2400);
DEFINE_NOTE(n2600, 8.0, 0.25, 2048, 2600);
DEFINE_NOTE(n2800, 8.0, 0.25, 2048, 2800);
DEFINE_NOTE(n3000, 8.0, 0.25, 2048, 3000);
DEFINE_NOTE(n3200, 8.0, 0.25, 2048, 3200);
DEFINE_NOTE(n3400, 8.0, 0.25, 2048, 3400);
DEFINE_NOTE(n3600, 8.0, 0.25, 2048, 3600);
DEFINE_NOTE(n3800, 8.0, 0.25, 2048, 3800);
DEFINE_NOTE(n4000, 8.0, 0.25, 2048, 4000);
DEFINE_NOTE(n4200, 8.0, 0.25, 2048, 4200);
DEFINE_NOTE(n4400, 8.0, 0.25, 2048, 4400);
DEFINE_NOTE(n4600, 8.0, 0.25, 2048, 4600);
DEFINE_NOTE(n4800, 8.0, 0.25, 2048, 4800);
DEFINE_NOTE(n5000, 8.0, 0.25, 2048, 5000);
Context context = {{n200, n400, n600, n800, n1000, n1200, n1400, n1600, n1800, n2000, n2200, n2400, n2600, n2800, n3000, n3200, n3400, n3600, n3800, n4000, n4200, n4400, n4600, n4800, n5000}, 0, 0};

void soundCallback(void *buffer_data, unsigned int frames) {
    PCM_SAMPLE_TYPE *frame_data = (PCM_SAMPLE_TYPE*)buffer_data;

    int true_off_period = context.notes[context.note_index].off_period / context.notes[context.note_index].period;
    true_off_period += (context.notes[context.note_index].off_period % context.notes[context.note_index].period) != 0;
    true_off_period *= context.notes[context.note_index].period;

    for( unsigned int f = 0; f < frames; f++ ) {
        if(context.time % context.notes[context.note_index].period > (context.notes[context.note_index].period / 2)) {
            frame_data[f] =  context.notes[context.note_index].amplitude;
        }
        else {
            frame_data[f] = -context.notes[context.note_index].amplitude;
        }
        //frame_data[f] = (double)context.notes[context.note_index].amplitude * sin(2. * M_PI * (context.time % context.notes[context.note_index].period) / (double)context.notes[context.note_index].period);

        if(context.time > true_off_period)
            frame_data[f] = 0;

        context.time++;

        if(context.time >= context.notes[context.note_index].on_period) {
            context.note_index++;

            if(context.note_index >= 24) {
                context.note_index = 0;
                true_off_period = context.notes[context.note_index].off_period / context.notes[context.note_index].period;
                true_off_period += (context.notes[context.note_index].off_period % context.notes[context.note_index].period) != 0;
                true_off_period *= context.notes[context.note_index].period;
            }

            context.time = 0;
        }
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
