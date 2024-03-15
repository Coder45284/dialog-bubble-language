#ifndef VOICE_FROM_TEXT_DTLWS
#define VOICE_FROM_TEXT_DTLWS

#include "voice.h"

typedef struct {
    VoiceContext voice_context;
    unsigned int length;
    unsigned int head;
    unsigned int volume;
    unsigned int min_frequency;
    unsigned int add_frequency;
    const char *text;
} VoiceTextContext;

void voiceFromTextSetup(VoiceTextContext *context);

unsigned int voiceFromTextSize(VoiceTextContext *context);
unsigned int voiceFromTextExportWAV(VoiceTextContext *context, const char *file_path);

#endif // VOICE_FROM_TEXT_DTLWS

