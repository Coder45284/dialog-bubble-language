#include <math.h>
#include <ctype.h>
#include <stddef.h>

#include "voice.h"

#define TIMES_A_SECOND( times_a_second ) ((float)PCM_SAMPLES_PER_SECOND / ((float)times_a_second))
#define FREQUENCY_TO_PERIOD( frequency ) ((float)PCM_SAMPLES_PER_SECOND / ((float)frequency))

#define DEFINE_NOTE(name, type, times_a_second, time_off, start_freq, end_freq, start_amp, end_amp) \
VoiceNote name = {type, TIMES_A_SECOND(times_a_second), TIMES_A_SECOND(times_a_second) * (time_off), FREQUENCY_TO_PERIOD(start_freq), FREQUENCY_TO_PERIOD(end_freq), start_amp, end_amp}

void voiceReadyContext(VoiceContext *context) {
    if(context->call_reloader != NULL && context->note_index == VOICE_NOTE_LIMIT)
        context->call_reloader(context);
    if(context->note_index >= context->note_amount) {
        context->note_index = 0;
    }

    context->note_state.type = context->notes[context->note_index].type;
    context->note_state.total_time = context->notes[context->note_index].total_time;
    context->note_state.sound_time = context->notes[context->note_index].sound_time;
    context->note_state.time = 0;
    context->note_state.period_begin = 0;
    context->note_state.current_period = context->notes[context->note_index].start_period;
    context->note_state.current_amplitude = context->notes[context->note_index].start_amp;
}

int voiceInputPhonemic(VoiceContext *context, const char *const string, unsigned int volume, unsigned int min_frequency, unsigned int add_frequency) {
    if(string[0] == '\0') {
        const DEFINE_NOTE(default_note, SQUARE, 8.0, 0.50, 60, 60, 0, 0);
        context->notes[context->note_amount] = default_note;
        context->note_amount++;
        return 1;
    }

    for(int i = 1; i < 3; i++) {
        if(string[i] == '\0')
            return 0;
    }

    const VoiceWaveType  wave_types[4] = {SINE, SQUARE, TRIANGLE, SAWTOOTH};
    const char wave_type_chars[4] = {'s', 'q', 't', 'w'};

    const unsigned int start_volumes[3] = {volume, volume, 0};
    const unsigned int   end_volumes[3] = {volume,      0, volume};
    const char         volumes_chars[3] = {'e', 'o', 'i'};

    const unsigned int   base_frequency = min_frequency + add_frequency / 2;
    const unsigned int   frequencies[3] = {base_frequency, min_frequency, min_frequency + add_frequency};
    const char       frequency_chars[3] = {'e', 'h', 'l'};

    VoiceWaveType    wave_type = SINE;
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
    context->notes[context->note_amount] = default_note;
    context->note_amount++;

    return 1;
}

void voiceInputPhonemics(VoiceContext *context, const char *const string, unsigned int length, unsigned int volume, unsigned int min_frequency, unsigned int add_frequency) {
    unsigned int n = 0;

    while(n != length) {
        if(string[n] == ' ') {
            voiceInputPhonemic(context, "", volume, min_frequency, add_frequency);
            n++;
        }
        else {
            voiceInputPhonemic(context, string + n, volume, min_frequency, add_frequency);

            n += 3;

            if(n > length)
                n = length;
        }
    }

    voiceReadyContext(context);
}

void voiceGenerateAllPhonemics(VoiceContext *context, unsigned int volume, unsigned int min_frequency, unsigned int add_frequency) {
    const char wave_type_chars[4] = {'s', 'q', 't', 'w'};
    const char   volumes_chars[3] = {'e', 'o', 'i'};
    const char frequency_chars[3] = {'e', 'h', 'l'};

    char phonem[4] = "see";

    context->note_amount = 0;

    for(int w = 0; w < 4; w++) {
        for(int f = 0; f < 3; f++) {
            for(int v = 0; v < 3; v++) {
                phonem[0] = wave_type_chars[w];
                phonem[1] =   volumes_chars[v];
                phonem[2] = frequency_chars[f];

                voiceInputPhonemic(context, phonem, volume, min_frequency, add_frequency);
            }
        }
    }
}

void voiceWriteToSoundBuffer(VoiceContext *context, void *buffer_data, unsigned int frames) {
    PCM_SAMPLE_TYPE *frame_data = (PCM_SAMPLE_TYPE*)buffer_data;
    PCM_SAMPLE_TYPE *current_frame_r;
    VoiceNoteState *note_r = &context->note_state;

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
                if(context->notes[context->note_index].start_period != context->notes[context->note_index].end_period)
                    note_r->current_period = context->notes[context->note_index].start_period * (1.0 - time) + context->notes[context->note_index].end_period * time;
                else
                    note_r->current_period = context->notes[context->note_index].start_period;

                if(context->notes[context->note_index].start_amp != context->notes[context->note_index].end_amp)
                    note_r->current_amplitude = context->notes[context->note_index].start_amp * (1.0 - time) + context->notes[context->note_index].end_amp * time;
                else
                    note_r->current_amplitude = context->notes[context->note_index].start_amp;
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
            context->note_index++;

            if(context->note_index == context->note_amount) {
                if(context->call_reloader != NULL && context->note_index == VOICE_NOTE_LIMIT)
                    context->call_reloader(context);
                else {
                    context->note_index = 0;
                    context->note_amount = 1;

                    context->notes[context->note_index].type = SQUARE;
                    context->notes[context->note_index].total_time = 5512;
                    context->notes[context->note_index].sound_time = 2756;
                    context->notes[context->note_index].start_period = 2048;
                    context->notes[context->note_index].end_period = 2048;
                    context->notes[context->note_index].start_amp = 0;
                    context->notes[context->note_index].end_amp = 0;
                }
            }

            voiceReadyContext(context);
        }
    }
}
