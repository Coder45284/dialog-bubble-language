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

    PCM_SAMPLE_TYPE max_amplitude;
    PCM_SAMPLE_TYPE min_amplitude;
} Note;

typedef struct {
    Wavetype type;
    int  on_period;
    int off_period;
    int period;
    int time;

    PCM_SAMPLE_TYPE current_amplitude;
} NoteState;

#define TIMES_A_SECOND( times_a_second ) ((float)PCM_SAMPLES_PER_SECOND / ((float)times_a_second))
#define FREQUENCY_TO_PERIOD( frequency ) ((float)PCM_SAMPLES_PER_SECOND / ((float)frequency))

#define DEFINE_NOTE(name, type, times_a_second, time_off, frequency, max_amp, min_amp) \
Note name = {type, TIMES_A_SECOND(times_a_second), TIMES_A_SECOND(times_a_second) * (time_off), FREQUENCY_TO_PERIOD(frequency), max_amp, min_amp}

typedef struct {
    Note notes[32];
    unsigned int note_index;

    NoteState note_state;
} Context;

const DEFINE_NOTE(default_note, SINE, 8.0, 0.25, 200, 2048, 1024);
Context context = {{}, 0, 0};

void soundCallback(void *buffer_data, unsigned int frames) {
    PCM_SAMPLE_TYPE *frame_data = (PCM_SAMPLE_TYPE*)buffer_data;
    PCM_SAMPLE_TYPE *current_frame_r;
    NoteState *note_r = &context.note_state;

    int true_off_period = note_r->off_period / note_r->period;
    true_off_period += (note_r->off_period % note_r->period) != 0;
    true_off_period *= note_r->period;

    for( unsigned int f = 0; f < frames; f++ ) {
        current_frame_r = &frame_data[f];

        switch(note_r->type) {
            case SINE:
            {
                *current_frame_r = (double)note_r->current_amplitude * sin(2. * M_PI * (note_r->time % note_r->period) / (double)note_r->period);
                break;
            }
            case SQUARE:
            {
                if(note_r->time % note_r->period > (note_r->period / 2))
                    *current_frame_r =  note_r->current_amplitude;
                else
                    *current_frame_r = -note_r->current_amplitude;
                break;
            }
            case TRIANGLE:
            {
                double time = ((note_r->time + 3 * note_r->period / 4) % note_r->period) / (double)note_r->period;

                if(time > 0.5)
                    *current_frame_r =  note_r->current_amplitude - 4.* (time - 0.5) * note_r->current_amplitude;
                else
                    *current_frame_r = -note_r->current_amplitude + 4. * time * note_r->current_amplitude;

                break;
            }
            case SAWTOOTH:
            {
                double time = (note_r->time % note_r->period) / (double)note_r->period;

                double displace = 2. * time * note_r->current_amplitude;

                if(displace > note_r->current_amplitude)
                    *current_frame_r = displace - 2. * note_r->current_amplitude;

                break;
            }
        }

        if(note_r->time > true_off_period)
            *current_frame_r = 0;

        note_r->time++;

        if(note_r->time >= note_r->on_period) {
            context.note_index++;

            if(context.note_index >= 32) {
                context.note_index = 0;

                true_off_period = note_r->off_period / note_r->period;
                true_off_period += (note_r->off_period % note_r->period) != 0;
                true_off_period *= note_r->period;
            }

            note_r->type = context.notes[context.note_index].type;
            note_r->on_period = context.notes[context.note_index].on_period;
            note_r->off_period = context.notes[context.note_index].off_period;
            note_r->period = context.notes[context.note_index].period;
            note_r->current_amplitude = context.notes[context.note_index].max_amplitude;

            note_r->time = 0;
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

    context.note_state.type = context.notes[context.note_index].type;
    context.note_state.on_period = context.notes[context.note_index].on_period;
    context.note_state.off_period = context.notes[context.note_index].off_period;
    context.note_state.period = context.notes[context.note_index].period;
    context.note_state.current_amplitude = context.notes[context.note_index].max_amplitude;

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
