#include "voice_from_text.h"

#include <raylib.h>
#include <stdlib.h>

static void callReloader(VoiceContext *voice) {
    voice->note_index = 0;
    voice->note_amount = 0;

    VoiceTextContext *context = (VoiceTextContext*)voice;

    while(context->head != context->length && context->voice_context.note_amount != VOICE_NOTE_LIMIT) {
        if(context->text[context->head] == ' ') {
            voiceInputPhonemic(&context->voice_context, "", context->volume, context->min_frequency, context->add_frequency);
            context->head++;
        }
        else {
            voiceInputPhonemic(&context->voice_context, &context->text[context->head], context->volume, context->min_frequency, context->add_frequency);

            context->head += 3;

            if(context->head > context->length)
                context->head = context->length;
        }
    }

    if(context->head == context->length) {
        context->voice_context.call_reloader = NULL;
    }

    voiceReadyContext(&context->voice_context);
}

void voiceFromTextSetup(VoiceTextContext *context) {
    context->head = 0;

    while(context->head != context->length && context->voice_context.note_amount != VOICE_NOTE_LIMIT) {
        if(context->text[context->head] == ' ') {
            voiceInputPhonemic(&context->voice_context, "", context->volume, context->min_frequency, context->add_frequency);
            context->head++;
        }
        else {
            voiceInputPhonemic(&context->voice_context, &context->text[context->head], context->volume, context->min_frequency, context->add_frequency);

            context->head += 3;

            if(context->head > context->length)
                context->head = context->length;
        }
    }

    voiceReadyContext(&context->voice_context);

    context->voice_context.call_reloader = callReloader;
}

unsigned int voiceFromTextSize(VoiceTextContext *context) {
    VoiceContext safe_context = context->voice_context;
    unsigned int head = 0;

    unsigned int frame_count = 0;

    while(head < context->length) {
        safe_context.note_index = 0;

        if(context->text[head] == ' ') {
            voiceInputPhonemic(&safe_context, "", context->volume, context->min_frequency, context->add_frequency);
            head++;
        }
        else {
            voiceInputPhonemic(&safe_context, &context->text[head], context->volume, context->min_frequency, context->add_frequency);

            head += 3;

            if(head > context->length)
                head = context->length;
        }

        frame_count += safe_context.notes[0].total_time; // Count the frames here.

        voiceReadyContext(&safe_context);
    }

    return frame_count;
}

unsigned int voiceFromTextExportWAV(VoiceTextContext *context, const char *file_path) {
    Wave wav;

    wav.frameCount = voiceFromTextSize(context);
    wav.sampleRate = PCM_SAMPLES_PER_SECOND;
    wav.sampleSize = PCM_SAMPLE_BITS;
    wav.channels   = 1;

    // No frames to write.
    if(wav.frameCount == 0)
        return false;

    voiceReadyContext(&context->voice_context);

    wav.data = malloc(wav.frameCount * sizeof(PCM_SAMPLE_TYPE));

    voiceWriteToSoundBuffer(&context->voice_context, wav.data, wav.frameCount);
    bool result = ExportWave(wav, file_path);

    free(wav.data);

    return result;
}

