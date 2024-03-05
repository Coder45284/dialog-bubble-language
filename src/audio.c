#include "audio.h"

VoiceContext audio_context = {{}, VOICE_NOTE_LIMIT, 0, 0};
AudioStream audio_stream;

static void audioSoundCallback(void *buffer_data, unsigned int frames) {
    voiceWriteToSoundBuffer(&audio_context, buffer_data, frames);
}

void audioInit() {
    audio_stream = LoadAudioStream(PCM_SAMPLES_PER_SECOND, PCM_SAMPLE_BITS, 1);
    SetAudioStreamCallback(audio_stream, audioSoundCallback);
}

void audioDeinit() {
    UnloadAudioStream(audio_stream); // Unload the voice data.
}
