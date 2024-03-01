#ifndef VOICE_DTLWS
#define VOICE_DTLWS

#include "constants.h"

typedef enum {
    SINE,
    SQUARE,
    TRIANGLE,
    SAWTOOTH
} VoiceWaveType;

typedef struct {
    VoiceWaveType type;
    int total_time;
    int sound_time;

    unsigned int    start_period;
    unsigned int      end_period;
    PCM_SAMPLE_TYPE start_amp;
    PCM_SAMPLE_TYPE   end_amp;
} Note;

typedef struct {
    VoiceWaveType type;
    int  total_time;
    int  sound_time;
    int        time;

    int                  period_begin;
    int                current_period;
    PCM_SAMPLE_TYPE current_amplitude;
} NoteState;

#define NOTE_LIMIT 512

typedef struct {
    Note notes[NOTE_LIMIT];
    unsigned int note_amount;
    unsigned int note_index;

    NoteState note_state;
} Context;

extern Context voiceContext;

void voiceReadyContext(Context *context);
int voiceInputPhonemic(const char *const string);
void voiceGenerateAllPhonemics();
void voiceSoundCallback(void *buffer_data, unsigned int frames);
int voiceExportWAV(const char *file_path);

#endif // VOICE_DTLWS

