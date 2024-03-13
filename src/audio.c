#include "audio.h"

#include "raylib.h"

mtx_t audio_context_mtx;
VoiceContext audio_context = {{}, VOICE_NOTE_LIMIT, 0, 0};

static AudioStream audio_stream;
static int audio_context_mtx_status;

static void audioSoundCallback(void *buffer_data, unsigned int frames) {
    voiceWriteToSoundBuffer(&audio_context, buffer_data, frames);
}

int audioInit() {
    audio_stream = LoadAudioStream(PCM_SAMPLES_PER_SECOND, PCM_SAMPLE_BITS, 1);
    SetAudioStreamCallback(audio_stream, audioSoundCallback);

    // Setup the audio_context mutex
    audio_context_mtx_status = mtx_init(&audio_context_mtx, mtx_plain);

    // Return true if the mutex succeeds.
    if(audio_context_mtx_status == thrd_success)
        return 1;
    else
        return 0;
}

void audioDeinit() {
    UnloadAudioStream(audio_stream); // Unload the voice data.

    if(audio_context_mtx_status == thrd_success)
        mtx_destroy(&audio_context_mtx);
}

void audioPlay() {
    PlayAudioStream(audio_stream);
}

void audioResume() {
    ResumeAudioStream(audio_stream);
}

void audioPause() {
    PauseAudioStream(audio_stream);
}

void audioStop() {
    StopAudioStream(audio_stream);
}
