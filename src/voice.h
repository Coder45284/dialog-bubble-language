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
} VoiceNote;

typedef struct {
    VoiceWaveType type;
    int  total_time;
    int  sound_time;
    int        time;

    int                  period_begin;
    int                current_period;
    PCM_SAMPLE_TYPE current_amplitude;
} VoiceNoteState;

#define VOICE_NOTE_LIMIT 512

typedef struct VoiceContext {
    VoiceNote notes[VOICE_NOTE_LIMIT];
    unsigned int note_amount;
    unsigned int note_index;

    VoiceNoteState note_state;

    void *data_ptr; // This points to additional metadata which only call_reloader deals with.

    void (*call_reloader)(struct VoiceContext *voice);
} VoiceContext;

void voiceReadyContext(VoiceContext *context);
int voiceInputPhonemic(VoiceContext *context, const char *const string, unsigned int volume, unsigned int min_frequency, unsigned int add_frequency);
void voiceInputPhonemics(VoiceContext *context, const char *const string, unsigned int length, unsigned int volume, unsigned int min_frequency, unsigned int add_frequency);
void voiceGenerateAllPhonemics(VoiceContext *context, unsigned int volume, unsigned int min_frequency, unsigned int add_frequency);
void voiceWriteToSoundBuffer(VoiceContext *context, void *buffer_data, unsigned int frames);
int voiceExportWAV(const char *file_path, unsigned int note_amount, VoiceNote *notes);

#endif // VOICE_DTLWS

