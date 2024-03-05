#ifndef AUDIO_DTLWS
#define AUDIO_DTLWS

#include "voice.h"

extern VoiceContext audio_context;

void audioInit();
void audioDeinit();

void audioPlay();
void audioResume();
void audioPause();
void audioStop();

#endif // AUDIO_DTLWS
