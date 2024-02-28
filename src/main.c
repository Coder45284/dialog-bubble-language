#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "constants.h"
#include "voice.h"

int main() {
    const int volume = 16384;

    Wavetype wave_types[4] = {SINE, SQUARE, TRIANGLE, SAWTOOTH};
    unsigned int   base_frequency = 1000;
    unsigned int   frequencies[3] = {base_frequency, 500, 1500};
    unsigned int start_volumes[3] = {volume, volume, 0};
    unsigned int   end_volumes[3] = {volume,      0, volume};

    const DEFINE_NOTE(default_note_0, SINE, 8.0, 0.50, 1000, 1000, volume, volume);
    const DEFINE_NOTE(default_note_1, SINE, 8.0, 0.50, 1000, 1000, volume,      0);
    const DEFINE_NOTE(default_note_2, SINE, 8.0, 0.50, 1000, 1000,      0, volume);
    const DEFINE_NOTE(default_note_3, SINE, 8.0, 0.50, 1000, 1500, volume, volume);
    const DEFINE_NOTE(default_note_4, SINE, 8.0, 0.50, 1000,  500, volume, volume);
    const DEFINE_NOTE(default_note_5, SINE, 8.0, 0.50, 1000, 1500, volume,      0);
    const DEFINE_NOTE(default_note_6, SINE, 8.0, 0.50, 1000,  500, volume,      0);

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

    // exportWAV("exp.wav");

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
