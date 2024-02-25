#include <raylib.h>
#include <stdio.h>
#include <math.h>

#include "constants.h"

typedef enum {
    SINE,
    SQUARE,
    TRIANGLE,
    SAWTOOTH
} Wavetype;

typedef struct {
    Wavetype type;
    int  on_period;
    int off_period;
    int period;
    PCM_SAMPLE_TYPE amplitude;
} Note;

#define TIMES_A_SECOND( times_a_second ) ((float)PCM_SAMPLES_PER_SECOND / ((float)times_a_second))

#define DEFINE_NOTE(name, type, times_a_second, time_off, amp, frequency) \
const Note name = {type, TIMES_A_SECOND(times_a_second), TIMES_A_SECOND(times_a_second) * (time_off), (float)PCM_SAMPLES_PER_SECOND / (frequency), amp}

#define SETUP_NOTE(note, times_a_second, time_off, amp, frequency) \
{\
    note.on_period  = TIMES_A_SECOND(times_a_second); \
    note.off_period = TIMES_A_SECOND(times_a_second) * (time_off); \
    note.period = (float)PCM_SAMPLES_PER_SECOND / (frequency); \
    note.amplitude = amp; \
}

typedef struct {
    Note notes[32];
    unsigned int note_index;
    int time;
} Context;

DEFINE_NOTE(default_note, SINE, 8.0, 0.25, 2048, 200);
Context context = {{}, 0, 0};

void soundCallback(void *buffer_data, unsigned int frames) {
    PCM_SAMPLE_TYPE *frame_data = (PCM_SAMPLE_TYPE*)buffer_data;
    PCM_SAMPLE_TYPE *current_frame_r;
    Note *note_r = &context.notes[context.note_index];

    int true_off_period = note_r->off_period / note_r->period;
    true_off_period += (note_r->off_period % note_r->period) != 0;
    true_off_period *= note_r->period;

    for( unsigned int f = 0; f < frames; f++ ) {
        current_frame_r = &frame_data[f];

        switch(note_r->type) {
            case SINE:
            {
                *current_frame_r = (double)note_r->amplitude * sin(2. * M_PI * (context.time % note_r->period) / (double)note_r->period);
                break;
            }
            case SQUARE:
            {
                if(context.time % note_r->period > (note_r->period / 2))
                    *current_frame_r =  note_r->amplitude;
                else
                    *current_frame_r = -note_r->amplitude;
                break;
            }
            case TRIANGLE:
            {
                double time = ((context.time + note_r->period / 4) % note_r->period) / (double)note_r->period;

                if(time > 0.5)
                    *current_frame_r =  note_r->amplitude - 4.* (time - 0.5) * note_r->amplitude;
                else
                    *current_frame_r = -note_r->amplitude + 4. * time * note_r->amplitude;

                break;
            }
            case SAWTOOTH:
            {
                double time = (context.time % note_r->period) / (double)note_r->period;

                double displace = 2. * time * note_r->amplitude;

                if(displace > note_r->amplitude)
                    *current_frame_r = displace - 2. * note_r->amplitude;

                break;
            }
        }

        if(context.time > true_off_period)
            *current_frame_r = 0;

        context.time++;

        if(context.time >= note_r->on_period) {
            context.note_index++;

            if(context.note_index >= 32) {
                context.note_index = 0;
                true_off_period = note_r->off_period / note_r->period;
                true_off_period += (note_r->off_period % note_r->period) != 0;
                true_off_period *= note_r->period;
            }

            note_r = &context.notes[context.note_index];

            context.time = 0;
        }
    }
}

int main() {
    Wavetype wave_types[4] = {SINE, SQUARE, TRIANGLE, SAWTOOTH};

    for(int i = 0; i < 32; i++) {
        context.notes[i] = default_note;
        context.notes[i].type = wave_types[i % 4];
        context.notes[i].period = (float)PCM_SAMPLES_PER_SECOND / (250 * (i / 4 + 1));
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
