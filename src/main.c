#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

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

    PCM_SAMPLE_TYPE start_amp;
    PCM_SAMPLE_TYPE   end_amp;
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
    unsigned int note_amount;
    unsigned int note_index;

    NoteState note_state;
} Context;

Context context = {{}, 32, 0, 0};

void soundCallback(void *buffer_data, unsigned int frames) {
    PCM_SAMPLE_TYPE *frame_data = (PCM_SAMPLE_TYPE*)buffer_data;
    PCM_SAMPLE_TYPE *current_frame_r;
    NoteState *note_r = &context.note_state;

    int true_off_period = note_r->off_period / note_r->period;
    true_off_period += (note_r->off_period % note_r->period) != 0;
    true_off_period *= note_r->period;

    for(unsigned int f = 0; f < frames; f++) {
        current_frame_r = &frame_data[f];

        {
            double time = note_r->time / (double)note_r->off_period;

            if( time > 1.0 )
                time = 1.0;

            note_r->current_amplitude = context.notes[context.note_index].start_amp * (1.0 - time) + context.notes[context.note_index].end_amp * time;
        }

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

                if( time > 1.0 )
                    time = 1.0;

                if(time > 0.5)
                    *current_frame_r =  note_r->current_amplitude - 4.* (time - 0.5) * note_r->current_amplitude;
                else
                    *current_frame_r = -note_r->current_amplitude + 4. * time * note_r->current_amplitude;

                break;
            }
            case SAWTOOTH:
            {
                double time = (note_r->time % note_r->period) / (double)note_r->period;

                *current_frame_r = -note_r->current_amplitude + 2.0 * note_r->current_amplitude * time;

                break;
            }
        }

        if(note_r->time > true_off_period)
            *current_frame_r = 0;

        note_r->time++;

        if(note_r->time >= note_r->on_period) {
            context.note_index++;

            if(context.note_index >= context.note_amount) {
                context.note_index = 0;

                true_off_period = note_r->off_period / note_r->period;
                true_off_period += (note_r->off_period % note_r->period) != 0;
                true_off_period *= note_r->period;
            }

            note_r->type = context.notes[context.note_index].type;
            note_r->on_period = context.notes[context.note_index].on_period;
            note_r->off_period = context.notes[context.note_index].off_period;
            note_r->period = context.notes[context.note_index].period;
            note_r->current_amplitude = context.notes[context.note_index].start_amp;

            note_r->time = 0;
        }
    }
}

int exportWAV(const char *file_path) {
    Wave wav;

    wav.frameCount = 0; // This will be found later.
    wav.sampleRate = PCM_SAMPLES_PER_SECOND;
    wav.sampleSize = PCM_SAMPLE_BITS;
    wav.channels   = 1;

    for(unsigned int n = 0; n < context.note_amount; n++) {
        wav.frameCount += context.notes[n].on_period;
    }

    wav.data = malloc(wav.frameCount * sizeof(PCM_SAMPLE_TYPE));

    soundCallback(wav.data, wav.frameCount);
    bool result = ExportWave(wav, file_path);

    free(wav.data);

    return result;
}

int main() {
    Wavetype wave_types[4] = {SINE, SQUARE, TRIANGLE, SAWTOOTH};
    const DEFINE_NOTE(default_note_0, SINE, 8.0, 0.50, 1000, 2048, 2048);
    const DEFINE_NOTE(default_note_1, SINE, 8.0, 0.50, 1000, 2048,    0);
    const DEFINE_NOTE(default_note_2, SINE, 8.0, 0.50, 1000,    0, 2048);

    const Note notes[3] = {default_note_0, default_note_1, default_note_2};

    context.note_amount = 0;

    for(int t = 0; t < 4; t++) {
        for(int i = 0; i < 3; i++) {
            context.notes[context.note_amount] = notes[i];
            context.notes[context.note_amount].type = wave_types[t];
            context.note_amount++;
        }
    }

    context.note_state.type = context.notes[context.note_index].type;
    context.note_state.on_period = context.notes[context.note_index].on_period;
    context.note_state.off_period = context.notes[context.note_index].off_period;
    context.note_state.period = context.notes[context.note_index].period;
    context.note_state.current_amplitude = context.notes[context.note_index].start_amp;

    exportWAV("exp.wav");

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
