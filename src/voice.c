#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

#include "voice.h"

#define TIMES_A_SECOND( times_a_second ) ((float)PCM_SAMPLES_PER_SECOND / ((float)times_a_second))
#define FREQUENCY_TO_PERIOD( frequency ) ((float)PCM_SAMPLES_PER_SECOND / ((float)frequency))

#define DEFINE_NOTE(name, type, times_a_second, time_off, start_freq, end_freq, start_amp, end_amp) \
Note name = {type, TIMES_A_SECOND(times_a_second), TIMES_A_SECOND(times_a_second) * (time_off), FREQUENCY_TO_PERIOD(start_freq), FREQUENCY_TO_PERIOD(end_freq), start_amp, end_amp}

Context voiceContext = {{}, NOTE_LIMIT, 0, 0};

void voiceReadyContext(Context *context) {
    if(context->note_index >= context->note_amount)
        context->note_index = 0;

    context->note_state.type = context->notes[context->note_index].type;
    context->note_state.total_time = context->notes[context->note_index].total_time;
    context->note_state.sound_time = context->notes[context->note_index].sound_time;
    context->note_state.time = 0;
    context->note_state.period_begin = 0;
    context->note_state.current_period = context->notes[context->note_index].start_period;
    context->note_state.current_amplitude = context->notes[context->note_index].start_amp;
}

int voiceInputPhonemic(const char *const string) {
    if(string[0] == '\0') {
        const DEFINE_NOTE(default_note, SQUARE, 8.0, 0.50, 60, 60, 0, 0);
        voiceContext.notes[voiceContext.note_amount] = default_note;
        voiceContext.note_amount++;
        return true;
    }

    for(int i = 1; i < 3; i++) {
        if(string[i] == '\0')
            return false;
    }

    const Wavetype  wave_types[4] = {SINE, SQUARE, TRIANGLE, SAWTOOTH};
    const char wave_type_chars[4] = {'s', 'q', 't', 'w'};

    const int                    volume = 16384;
    const unsigned int start_volumes[3] = {volume, volume, 0};
    const unsigned int   end_volumes[3] = {volume,      0, volume};
    const char         volumes_chars[3] = {   'e',    'o', 'i'};

    const unsigned int   base_frequency = 1000;
    const unsigned int   frequencies[3] = {base_frequency, 500, 1500};
    const char       frequency_chars[3] = {           'e', 'h', 'l'};

    Wavetype         wave_type = SINE;
    unsigned int  start_volume = volume;
    unsigned int    end_volume = volume;
    unsigned int end_frequency = base_frequency;

    for(int w = 0; w < 4; w++) {
        if(wave_type_chars[w] == tolower(string[0]) )
            wave_type = wave_types[w];
    }

    for(int v = 0; v < 3; v++) {
        if(volumes_chars[v] == tolower(string[1]) ) {
            start_volume = start_volumes[v];
            end_volume   =   end_volumes[v];
        }
    }

    for(int f = 0; f < 3; f++) {
        if(frequency_chars[f] == tolower(string[2]) )
            end_frequency = frequencies[f];
    }

    const DEFINE_NOTE(default_note, wave_type, 8.0, 0.50, base_frequency, end_frequency, start_volume, end_volume);
    voiceContext.notes[voiceContext.note_amount] = default_note;
    voiceContext.note_amount++;

    return true;
}

void voiceGenerateAllPhonemics() {
    const char wave_type_chars[4] = {'s', 'q', 't', 'w'};
    const char   volumes_chars[3] = {'e', 'o', 'i'};
    const char frequency_chars[3] = {'e', 'h', 'l'};

    char phonem[4] = "see";

    voiceContext.note_amount = 0;

    for(int w = 0; w < 4; w++) {
        for(int f = 0; f < 3; f++) {
            for(int v = 0; v < 3; v++) {
                phonem[0] = wave_type_chars[w];
                phonem[1] =   volumes_chars[v];
                phonem[2] = frequency_chars[f];

                voiceInputPhonemic(phonem);
            }
        }
    }
}

void voiceSoundCallback(void *buffer_data, unsigned int frames) {
    PCM_SAMPLE_TYPE *frame_data = (PCM_SAMPLE_TYPE*)buffer_data;
    PCM_SAMPLE_TYPE *current_frame_r;
    NoteState *note_r = &voiceContext.note_state;

    for(unsigned int f = 0; f < frames; f++) {
        current_frame_r = &frame_data[f];

        if(note_r->time - note_r->period_begin >= note_r->current_period ) {
            double time = note_r->time / (double)note_r->sound_time;

            if( time > 1.0 )
                time = 1.0;

            note_r->period_begin = note_r->time;

            // Silince the note.
            if(note_r->time >= note_r->sound_time) {
                note_r->current_period = note_r->total_time - note_r->period_begin;
                note_r->current_amplitude = 0;
            }
            else {
                if(voiceContext.notes[voiceContext.note_index].start_period != voiceContext.notes[voiceContext.note_index].end_period)
                    note_r->current_period = voiceContext.notes[voiceContext.note_index].start_period * (1.0 - time) + voiceContext.notes[voiceContext.note_index].end_period * time;
                else
                    note_r->current_period = voiceContext.notes[voiceContext.note_index].start_period;

                if(voiceContext.notes[voiceContext.note_index].start_amp != voiceContext.notes[voiceContext.note_index].end_amp)
                    note_r->current_amplitude = voiceContext.notes[voiceContext.note_index].start_amp * (1.0 - time) + voiceContext.notes[voiceContext.note_index].end_amp * time;
                else
                    note_r->current_amplitude = voiceContext.notes[voiceContext.note_index].start_amp;
            }
        }

        switch(note_r->type) {
            case SINE:
            {
                *current_frame_r = (double)note_r->current_amplitude * sin(2. * M_PI * (note_r->time - note_r->period_begin) / (double)note_r->current_period);
                break;
            }
            case SQUARE:
            {
                if(note_r->time - note_r->period_begin > (note_r->current_period / 2))
                    *current_frame_r =  note_r->current_amplitude;
                else
                    *current_frame_r = -note_r->current_amplitude;
                break;
            }
            case TRIANGLE:
            {
                double time = ((note_r->time - note_r->period_begin + 3 * note_r->current_period / 4) % note_r->current_period) / (double)note_r->current_period;

                if(time > 0.5)
                    *current_frame_r =  note_r->current_amplitude - 4.* (time - 0.5) * note_r->current_amplitude;
                else
                    *current_frame_r = -note_r->current_amplitude + 4. * time * note_r->current_amplitude;

                break;
            }
            case SAWTOOTH:
            {
                double time = ((note_r->time - note_r->period_begin + note_r->current_period / 2) % note_r->current_period) / (double)note_r->current_period;

                *current_frame_r = -note_r->current_amplitude + 2.0 * note_r->current_amplitude * time;

                break;
            }
        }

        note_r->time++;

        if(note_r->time >= note_r->total_time) {
            voiceContext.note_index++;

            if(voiceContext.note_index == voiceContext.note_amount) {
                voiceContext.note_index = 0;
                voiceContext.note_amount = 1;

                voiceContext.notes[voiceContext.note_index].type = SQUARE;
                voiceContext.notes[voiceContext.note_index].start_amp = 0;
                voiceContext.notes[voiceContext.note_index].end_amp = 0;
            }

            voiceReadyContext(&voiceContext);
        }
    }
}

int voiceExportWAV(const char *file_path) {
    Wave wav;

    wav.frameCount = 0; // This will be found later.
    wav.sampleRate = PCM_SAMPLES_PER_SECOND;
    wav.sampleSize = PCM_SAMPLE_BITS;
    wav.channels   = 1;

    for(unsigned int n = 0; n < voiceContext.note_amount; n++) {
        wav.frameCount += voiceContext.notes[n].total_time;
    }

    wav.data = malloc(wav.frameCount * sizeof(PCM_SAMPLE_TYPE));

    voiceSoundCallback(wav.data, wav.frameCount);
    bool result = ExportWave(wav, file_path);

    free(wav.data);

    return result;
}

