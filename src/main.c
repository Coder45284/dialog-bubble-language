#include <raylib.h>
#include <stdio.h>
#include <math.h>

#include "constants.h"

typedef enum {
    SINE,
    SQUARE
} Wavetype;

typedef struct {
    Wavetype type;
    int  on_period;
    int off_period;
    int period;
    PCM_SAMPLE_TYPE amplitude;
} Note;

#define DEFINE_NOTE(name, times_a_second, time_off, amp, frequency) \
const Note name = {SINE, (float)PCM_SAMPLES_PER_SECOND / ((float)times_a_second), ((float)PCM_SAMPLES_PER_SECOND / (float)times_a_second) * (time_off), (float)PCM_SAMPLES_PER_SECOND / (frequency), amp}

#define SETUP_NOTE(note, times_a_second, time_off, amp, frequency) \
{\
    note.on_period  = (float)PCM_SAMPLES_PER_SECOND / ((float)times_a_second); \
    note.off_period = ((float)PCM_SAMPLES_PER_SECOND / (float)times_a_second) * (time_off); \
    note.period = (float)PCM_SAMPLES_PER_SECOND / (frequency); \
    note.amplitude = amp; \
}

typedef struct {
    Note notes[32];
    unsigned int note_index;
    int time;
} Context;

DEFINE_NOTE(base, 8.0, 0.25, 2048, 200);
Context context = {{}, 0, 0};

void soundCallback(void *buffer_data, unsigned int frames) {
    PCM_SAMPLE_TYPE *frame_data = (PCM_SAMPLE_TYPE*)buffer_data;

    int true_off_period = context.notes[context.note_index].off_period / context.notes[context.note_index].period;
    true_off_period += (context.notes[context.note_index].off_period % context.notes[context.note_index].period) != 0;
    true_off_period *= context.notes[context.note_index].period;

    for( unsigned int f = 0; f < frames; f++ ) {
        switch( context.notes[context.note_index].type) {
            case SQUARE:
            {
                if(context.time % context.notes[context.note_index].period > (context.notes[context.note_index].period / 2))
                    frame_data[f] =  context.notes[context.note_index].amplitude;
                else
                    frame_data[f] = -context.notes[context.note_index].amplitude;
                break;
            }
            case SINE:
            {
                frame_data[f] = (double)context.notes[context.note_index].amplitude * sin(2. * M_PI * (context.time % context.notes[context.note_index].period) / (double)context.notes[context.note_index].period);
                break;
            }
        }

        if(context.time > true_off_period)
            frame_data[f] = 0;

        context.time++;

        if(context.time >= context.notes[context.note_index].on_period) {
            context.note_index++;

            if(context.note_index >= 32) {
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
    Wavetype wave_types[2] = {SINE, SQUARE};

    for(int i = 0; i < 32; i++) {
        context.notes[i] = base;
        context.notes[i].type = wave_types[i % 2];
        context.notes[i].period = (float)PCM_SAMPLES_PER_SECOND / (250 * (i / 2 + 1));
    }

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
