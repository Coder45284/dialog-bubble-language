#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "constants.h"
#include "voice.h"

int main() {
    Wavetype wave_types[4] = {SINE, SQUARE, TRIANGLE, SAWTOOTH};

    unsigned int   base_frequency = 1000;
    unsigned int   frequencies[3] = {base_frequency, 500, 1500};

    const int volume = 16384;
    unsigned int start_volumes[3] = {volume, volume, 0};
    unsigned int   end_volumes[3] = {volume,      0, volume};

    context.note_amount = 0;

    for(int w = 0; w < 4; w++) {
        for(int f = 0; f < 3; f++) {
            for(int v = 0; v < 3; v++) {
                const DEFINE_NOTE(default_note, wave_types[w], 8.0, 0.50, base_frequency, frequencies[f], start_volumes[v], end_volumes[v]);
                context.notes[context.note_amount] = default_note;
                context.note_amount++;
            }
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
