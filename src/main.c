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
    Wavetype  type;
    int total_time;
    int sound_time;

    unsigned int    start_period;
    unsigned int      end_period;
    PCM_SAMPLE_TYPE start_amp;
    PCM_SAMPLE_TYPE   end_amp;
} Note;

typedef struct {
    Wavetype   type;
    int  total_time;
    int  sound_time;
    int        time;

    int                  period_begin;
    int                current_period;
    PCM_SAMPLE_TYPE current_amplitude;
} NoteState;

#define TIMES_A_SECOND( times_a_second ) ((float)PCM_SAMPLES_PER_SECOND / ((float)times_a_second))
#define FREQUENCY_TO_PERIOD( frequency ) ((float)PCM_SAMPLES_PER_SECOND / ((float)frequency))

#define DEFINE_NOTE(name, type, times_a_second, time_off, start_freq, end_freq, start_amp, end_amp) \
Note name = {type, TIMES_A_SECOND(times_a_second), TIMES_A_SECOND(times_a_second) * (time_off), FREQUENCY_TO_PERIOD(start_freq), FREQUENCY_TO_PERIOD(end_freq), start_amp, end_amp}

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

    for(unsigned int f = 0; f < frames; f++) {
        current_frame_r = &frame_data[f];

        if(note_r->time - note_r->period_begin >= note_r->current_period ){
            double time = note_r->time / (double)note_r->sound_time;

            if( time > 1.0 )
                time = 1.0;

            if(context.notes[context.note_index].start_period != context.notes[context.note_index].end_period)
                note_r->current_period = context.notes[context.note_index].start_period * (1.0 - time) + context.notes[context.note_index].end_period * time;
            else
                note_r->current_period = context.notes[context.note_index].start_period;

            if(context.notes[context.note_index].start_amp != context.notes[context.note_index].end_amp)
                note_r->current_amplitude = context.notes[context.note_index].start_amp * (1.0 - time) + context.notes[context.note_index].end_amp * time;
            else
                note_r->current_amplitude = context.notes[context.note_index].start_amp;

            note_r->period_begin = note_r->time;
        }

        switch(note_r->type) {
            case SINE:
            {
                *current_frame_r = (double)note_r->current_amplitude * sin(2. * M_PI * (note_r->time - note_r->period_begin) / (double)note_r->current_period);
                break;
            }
            case SQUARE:
            {
                if(note_r->time - note_r->period_begin > (note_r->current_period / 2))
                    *current_frame_r =  note_r->current_amplitude;
                else
                    *current_frame_r = -note_r->current_amplitude;
                break;
            }
            case TRIANGLE:
            {
                double time = ((note_r->time - note_r->period_begin + 3 * note_r->current_period / 4) % note_r->current_period) / (double)note_r->current_period;

                if(time > 0.5)
                    *current_frame_r =  note_r->current_amplitude - 4.* (time - 0.5) * note_r->current_amplitude;
                else
                    *current_frame_r = -note_r->current_amplitude + 4. * time * note_r->current_amplitude;

                break;
            }
            case SAWTOOTH:
            {
                double time = ((note_r->time - note_r->period_begin + note_r->current_period / 2) % note_r->current_period) / (double)note_r->current_period;

                *current_frame_r = -note_r->current_amplitude + 2.0 * note_r->current_amplitude * time;

                break;
            }
        }

        if(note_r->time > note_r->sound_time)
            *current_frame_r = 0;

        note_r->time++;

        if(note_r->time >= note_r->total_time) {
            context.note_index++;

            if(context.note_index >= context.note_amount) {
                context.note_index = 0;
            }

            note_r->type = context.notes[context.note_index].type;
            note_r->total_time = context.notes[context.note_index].total_time;
            note_r->sound_time = context.notes[context.note_index].sound_time;
            note_r->period_begin = 0;
            note_r->current_period = context.notes[context.note_index].start_period;
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
        wav.frameCount += context.notes[n].total_time;
    }

    wav.data = malloc(wav.frameCount * sizeof(PCM_SAMPLE_TYPE));

    soundCallback(wav.data, wav.frameCount);
    bool result = ExportWave(wav, file_path);

    free(wav.data);

    return result;
}

int main() {
    Wavetype wave_types[4] = {SINE, SQUARE, TRIANGLE, SAWTOOTH};
    const DEFINE_NOTE(default_note_0, SINE, 8.0, 0.50, 1000, 1000, 2048, 2048);
    const DEFINE_NOTE(default_note_1, SINE, 8.0, 0.50, 1000, 1000, 2048,    0);
    const DEFINE_NOTE(default_note_2, SINE, 8.0, 0.50, 1000, 1000,    0, 2048);
    const DEFINE_NOTE(default_note_3, SINE, 8.0, 0.50, 1000, 1500, 2048, 2048);
    const DEFINE_NOTE(default_note_4, SINE, 8.0, 0.50, 1500, 1000, 2048, 2048);
    const DEFINE_NOTE(default_note_5, SINE, 8.0, 0.50, 1000,  500, 2048, 2048);
    const DEFINE_NOTE(default_note_6, SINE, 8.0, 0.50,  500, 1000, 2048, 2048);

    const Note notes[7] = {default_note_0, default_note_1, default_note_2, default_note_3, default_note_4, default_note_5, default_note_6};

    context.note_amount = 0;

    for(int t = 0; t < 4; t++) {
        for(int i = 0; i < 7; i++) {
            context.notes[context.note_amount] = notes[i];
            context.notes[context.note_amount].type = wave_types[t];
            context.note_amount++;
        }
    }

    context.note_state.type = context.notes[context.note_index].type;
    context.note_state.total_time = context.notes[context.note_index].total_time;
    context.note_state.sound_time = context.notes[context.note_index].sound_time;
    context.note_state.period_begin = 0;
    context.note_state.current_period = context.notes[context.note_index].start_period;
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
