#include <raylib.h>
#include <math.h>
#include <stdlib.h>

#include "voice.h"

Context context = {{}, 32, 0, 0};

void soundCallback(void *buffer_data, unsigned int frames) {
    PCM_SAMPLE_TYPE *frame_data = (PCM_SAMPLE_TYPE*)buffer_data;
    PCM_SAMPLE_TYPE *current_frame_r;
    NoteState *note_r = &context.note_state;

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
                if(context.notes[context.note_index].start_period != context.notes[context.note_index].end_period)
                    note_r->current_period = context.notes[context.note_index].start_period * (1.0 - time) + context.notes[context.note_index].end_period * time;
                else
                    note_r->current_period = context.notes[context.note_index].start_period;

                if(context.notes[context.note_index].start_amp != context.notes[context.note_index].end_amp)
                    note_r->current_amplitude = context.notes[context.note_index].start_amp * (1.0 - time) + context.notes[context.note_index].end_amp * time;
                else
                    note_r->current_amplitude = context.notes[context.note_index].start_amp;
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
            context.note_index++;

            if(context.note_index >= context.note_amount) {
                context.note_index = 0;
            }

            note_r->type = context.notes[context.note_index].type;
            note_r->total_time = context.notes[context.note_index].total_time;
            note_r->sound_time = context.notes[context.note_index].sound_time;
            note_r->period_begin = 0;
            note_r->current_period = context.notes[context.note_index].start_period;
            note_r->current_amplitude = context.notes[context.note_index].start_amp;

            note_r->time = 0;
        }
    }
}

int exportWAV(const char *file_path) {
    Wave wav;

    wav.frameCount = 0; // This will be found later.
    wav.sampleRate = PCM_SAMPLES_PER_SECOND;
    wav.sampleSize = PCM_SAMPLE_BITS;
    wav.channels   = 1;

    for(unsigned int n = 0; n < context.note_amount; n++) {
        wav.frameCount += context.notes[n].total_time;
    }

    wav.data = malloc(wav.frameCount * sizeof(PCM_SAMPLE_TYPE));

    soundCallback(wav.data, wav.frameCount);
    bool result = ExportWave(wav, file_path);

    free(wav.data);

    return result;
}

