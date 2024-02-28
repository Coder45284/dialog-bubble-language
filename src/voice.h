#ifndef VOICE_DTLWS
#define VOICE_DTLWS

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

typedef struct {
    Note notes[128];
    unsigned int note_amount;
    unsigned int note_index;

    NoteState note_state;
} Context;

extern Context context;

#define TIMES_A_SECOND( times_a_second ) ((float)PCM_SAMPLES_PER_SECOND / ((float)times_a_second))
#define FREQUENCY_TO_PERIOD( frequency ) ((float)PCM_SAMPLES_PER_SECOND / ((float)frequency))

#define DEFINE_NOTE(name, type, times_a_second, time_off, start_freq, end_freq, start_amp, end_amp) \
Note name = {type, TIMES_A_SECOND(times_a_second), TIMES_A_SECOND(times_a_second) * (time_off), FREQUENCY_TO_PERIOD(start_freq), FREQUENCY_TO_PERIOD(end_freq), start_amp, end_amp}

void soundCallback(void *buffer_data, unsigned int frames);
int exportWAV(const char *file_path);

#endif // VOICE_DTLWS

