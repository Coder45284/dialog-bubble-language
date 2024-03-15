#ifndef AUDIO_DTLWS
#define AUDIO_DTLWS

#include "voice_from_text.h"
#include <threads.h>

extern mtx_t audio_context_mtx;
extern VoiceTextContext audio_context;

int audioInit();
void audioDeinit();

void audioPlay();
void audioResume();
void audioPause();
void audioStop();

#endif // AUDIO_DTLWS
