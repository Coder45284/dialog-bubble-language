#ifndef AUDIO_DTLWS
#define AUDIO_DTLWS

#include "raylib.h"

#include "voice.h"

extern VoiceContext audio_context;
extern AudioStream audio_stream;

void audioInit();
void audioDeinit();

#endif // AUDIO_DTLWS
