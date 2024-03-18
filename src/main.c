#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "constants.h"
#include "voice.h"
#include "voice_from_text.h"
#include "audio.h"
#include "grammer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------
static void ButtonWordSearchEnglish();
static void ButtonWordSearchLanguage();
static void ButtonDictionaryUpdateEntry();
static void ButtonDictionaryDeleteEntry();
static void ButtonDictionaryPlaySound();
static void ButtonWordGeneratorGenerate();
static void ButtonGeneratorWordReplace();
static void ButtonVoiceNoiseTest();
static void ButtonGrammer();
static void ButtonLanguageSound();
static void ButtonLanguageSoundExport();

const char *DropDownBoxLanguageSoundFormatText = "WAV;TXT"; // FLAC and OGG and VOA where options, but then I thought better of it.
int DropDownBoxLanguageSoundFormatActive = 0;

int ValueBoxVoiceVolumeValue  = 16384;
int ValueBoxVoiceFreqValue      = 500;
int ValueBoxVoiceFreqPlusValue = 1000;

char TextBoxLanguageEntryText[128] = "";

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //---------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dialog Text Language Work Station");

    // Sound Initialization
    //---------------------------------------------------------------------------------------
    InitAudioDevice();
    SetAudioStreamBufferSizeDefault(VOICE_BUFFER_SIZE);

    if(audioInit() == 0) {
        printf("Audio initization has failed! It might be because the program has failed to make a mutex.\n");

        CloseAudioDevice();       // Close sound and sound context
        CloseWindow();            // Close window and OpenGL context

        return -1;
    }

    // language_builder: controls initialization
    //----------------------------------------------------------------------------------
    const char *GroupBoxWordSearchText = "Word Search";
    const char *LabelWordSearchText = "English or Language";
    const char *ButtonWordSearchEnglishText = "English";
    const char *ButtonWordSearchLanguageText = "Language";
    const char *GroupBoxDictionaryText = "Dictionary";
    const char *LabelDictionaryLWordText = "Language Word:";
    const char *LabelDictionaryEWordText = "English Keyword:";
    const char *LabelDictionaryPOSText = "Parts of Speech:";
    const char *LabelDictionaryEDescriptionText = "English Definition:";
    const char *ButtonDictionaryUpdateEntryText = "Update";
    const char *ButtonDictionaryDeleteEntryText = "Delete";
    const char *ButtonDictionaryPlaySoundText = "Play Sound";
    const char *GroupBoxWordGeneratorText = "Word Generator";
    const char *LabelWordGeneratorPOSText = "Parts of Speech";
    const char *DropDownBoxWordGeneratorTypeText = "NOUN;ADJECTIVE;VERB;ADVERB;OTHER";
    const char *ButtonWordGeneratorGenerateText = "Generate";
    const char *LabelWordGeneratorWordSelectionText = "Word Options";
    const char *DropDownBoxGeneratorWordSelectionText = "ONE;TWO;THREE";
    const char *LabelGeneratorWordPlacementText = "Place Into Language";
    const char *ButtonGeneratorWordReplaceText = "Replace Word";
    const char *GroupBoxVoiceText = "Voice Options";
    const char *LabelVoiceNoiseTypeText = "Noise Type";
    const char *DropDownBoxVoiceNoiseTypeText = "WHITE;PINK;BROWN;BLUE";
    const char *ButtonVoiceNoiseTestText = "Test Every Note";
    const char *GroupBoxLanguageText = "Language Snippit Editor";
    const char *ButtonGrammerText = "Check Grammer";
    const char *ButtonLanguageSoundText = "Play Sound";
    const char *ButtonLanguageSoundExportText = "Export Sound";
    const char *ScrollPanelWordSearchQueryText = "Query";
    const char *ValueBoxVoiceVolumeText = "Peak Volume";
    const char *ValueBoxVoiceFreqText = "Frequency(Hz)";
    const char *ValueBoxVoiceFreqPlusText = "Plus(Hz)";
    const char *ValueBoxVoiceNoiseVolumeText = "Noise Volume";
    
    bool TextBoxWordSearchEditMode = false;
    char TextBoxWordSearchText[128] = "";
    Rectangle ScrollPanelWordSearchQueryScrollView = { 0, 0, 0, 0 };
    Vector2 ScrollPanelWordSearchQueryScrollOffset = { 0, 0 };
    Vector2 ScrollPanelWordSearchQueryBoundsOffset = { 0, 0 };
    bool TextBoxDictionaryLWordEditMode = false;
    char TextBoxDictionaryLWordText[128] = "";
    bool TextBoxDictionaryEWordEditMode = false;
    char TextBoxDictionaryEWordText[128] = "";
    bool TextBoxDictionaryPOSEditMode = false;
    char TextBoxDictionaryPOSText[128] = "";
    bool TextBoxDictionaryEDescriptionEditMode = false;
    char TextBoxDictionaryEDescriptionText[128] = "";
    bool DropDownBoxWordGeneratorTypeEditMode = false;
    int DropDownBoxWordGeneratorTypeActive = 0;
    bool DropDownBoxGeneratorWordSelectionEditMode = false;
    int DropDownBoxGeneratorWordSelectionActive = 0;
    bool ValueBoxVoiceVolumeEditMode = false;
    bool ValueBoxVoiceFreqEditMode = false;
    bool ValueBoxVoiceFreqPlusEditMode = false;
    bool ValueBoxVoiceNoiseVolumeEditMode = false;
    int ValueBoxVoiceNoiseVolumeValue = 0;
    bool DropDownBoxVoiceNoiseTypeEditMode = false;
    int DropDownBoxVoiceNoiseTypeActive = 0;
    bool TextBoxLanguageEntryEditMode = false;
    bool DropDownBoxLanguageSoundFormatEditMode = false;
    //----------------------------------------------------------------------------------

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Implement required update logic
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 

            // raygui: controls drawing
            //----------------------------------------------------------------------------------
            if (DropDownBoxWordGeneratorTypeEditMode || DropDownBoxGeneratorWordSelectionEditMode || DropDownBoxVoiceNoiseTypeEditMode || DropDownBoxLanguageSoundFormatEditMode) GuiLock();

            GuiGroupBox((Rectangle){ 0, 8, 216, 256 }, GroupBoxWordSearchText);
            if (GuiTextBox((Rectangle){ 24, 24, 168, 24 }, TextBoxWordSearchText, 128, TextBoxWordSearchEditMode)) TextBoxWordSearchEditMode = !TextBoxWordSearchEditMode;
            GuiLabel((Rectangle){ 24, 72, 168, 24 }, LabelWordSearchText);
            if (GuiButton((Rectangle){ 24, 96, 72, 24 }, ButtonWordSearchEnglishText)) ButtonWordSearchEnglish(); 
            if (GuiButton((Rectangle){ 120, 96, 72, 24 }, ButtonWordSearchLanguageText)) ButtonWordSearchLanguage(); 
            GuiScrollPanel((Rectangle){ 24, 144, 168 - ScrollPanelWordSearchQueryBoundsOffset.x, 96 - ScrollPanelWordSearchQueryBoundsOffset.y }, ScrollPanelWordSearchQueryText, (Rectangle){ 24, 144, 168, 96 }, &ScrollPanelWordSearchQueryScrollOffset, &ScrollPanelWordSearchQueryScrollView);
            GuiGroupBox((Rectangle){ 216, 8, 456, 256 }, GroupBoxDictionaryText);
            GuiLabel((Rectangle){ 240, 24, 96, 24 }, LabelDictionaryLWordText);
            if (GuiTextBox((Rectangle){ 336, 24, 312, 24 }, TextBoxDictionaryLWordText, 128, TextBoxDictionaryLWordEditMode)) TextBoxDictionaryLWordEditMode = !TextBoxDictionaryLWordEditMode;
            GuiLabel((Rectangle){ 240, 48, 96, 24 }, LabelDictionaryEWordText);
            if (GuiTextBox((Rectangle){ 336, 48, 312, 24 }, TextBoxDictionaryEWordText, 128, TextBoxDictionaryEWordEditMode)) TextBoxDictionaryEWordEditMode = !TextBoxDictionaryEWordEditMode;
            GuiLabel((Rectangle){ 240, 72, 96, 24 }, LabelDictionaryPOSText);
            if (GuiTextBox((Rectangle){ 336, 72, 312, 24 }, TextBoxDictionaryPOSText, 128, TextBoxDictionaryPOSEditMode)) TextBoxDictionaryPOSEditMode = !TextBoxDictionaryPOSEditMode;
            GuiLabel((Rectangle){ 240, 96, 120, 24 }, LabelDictionaryEDescriptionText);
            if (GuiTextBox((Rectangle){ 240, 120, 408, 72 }, TextBoxDictionaryEDescriptionText, 128, TextBoxDictionaryEDescriptionEditMode)) TextBoxDictionaryEDescriptionEditMode = !TextBoxDictionaryEDescriptionEditMode;
            if (GuiButton((Rectangle){ 240, 216, 120, 24 }, ButtonDictionaryUpdateEntryText)) ButtonDictionaryUpdateEntry(); 
            if (GuiButton((Rectangle){ 384, 216, 120, 24 }, ButtonDictionaryDeleteEntryText)) ButtonDictionaryDeleteEntry(); 
            if (GuiButton((Rectangle){ 528, 216, 120, 24 }, ButtonDictionaryPlaySoundText)) ButtonDictionaryPlaySound(); 
            GuiGroupBox((Rectangle){ 672, 8, 120, 256 }, GroupBoxWordGeneratorText);
            GuiLabel((Rectangle){ 688, 24, 88, 24 }, LabelWordGeneratorPOSText);
            if (GuiButton((Rectangle){ 696, 104, 72, 24 }, ButtonWordGeneratorGenerateText)) ButtonWordGeneratorGenerate(); 
            GuiLabel((Rectangle){ 696, 144, 72, 24 }, LabelWordGeneratorWordSelectionText);
            GuiLabel((Rectangle){ 680, 200, 104, 24 }, LabelGeneratorWordPlacementText);
            if (GuiButton((Rectangle){ 688, 224, 88, 24 }, ButtonGeneratorWordReplaceText)) ButtonGeneratorWordReplace(); 
            GuiGroupBox((Rectangle){ 0, 272, 216, 208 }, GroupBoxVoiceText);
            if (GuiValueBox((Rectangle){ 104, 288, 88, 24 }, ValueBoxVoiceVolumeText, &ValueBoxVoiceVolumeValue, 0, 0x7FFF, ValueBoxVoiceVolumeEditMode)) ValueBoxVoiceVolumeEditMode = !ValueBoxVoiceVolumeEditMode;
            if (GuiValueBox((Rectangle){ 104, 312, 88, 24 }, ValueBoxVoiceFreqText, &ValueBoxVoiceFreqValue, 60, 2000, ValueBoxVoiceFreqEditMode)) ValueBoxVoiceFreqEditMode = !ValueBoxVoiceFreqEditMode;
            if (GuiValueBox((Rectangle){ 104, 336, 88, 24 }, ValueBoxVoiceFreqPlusText, &ValueBoxVoiceFreqPlusValue, 60, 3000, ValueBoxVoiceFreqPlusEditMode)) ValueBoxVoiceFreqPlusEditMode = !ValueBoxVoiceFreqPlusEditMode;
            if (GuiValueBox((Rectangle){ 104, 360, 88, 24 }, ValueBoxVoiceNoiseVolumeText, &ValueBoxVoiceNoiseVolumeValue, 0, 100, ValueBoxVoiceNoiseVolumeEditMode)) ValueBoxVoiceNoiseVolumeEditMode = !ValueBoxVoiceNoiseVolumeEditMode;

            GuiLabel((Rectangle){ 40, 384, 64, 24 }, LabelVoiceNoiseTypeText);
            if (GuiButton((Rectangle){ 24, 432, 168, 24 }, ButtonVoiceNoiseTestText)) ButtonVoiceNoiseTest(); 
            GuiGroupBox((Rectangle){ 216, 272, 576, 208 }, GroupBoxLanguageText);
            GuiLabel((Rectangle){ 240, 288, 64, 24 }, "Word Input:");
            if (GuiTextBox((Rectangle){ 240, 312, 528, 24 }, TextBoxLanguageEntryText, 128, TextBoxLanguageEntryEditMode)) TextBoxLanguageEntryEditMode = !TextBoxLanguageEntryEditMode;
            if (GuiButton((Rectangle){ 240, 432, 96, 24 }, ButtonGrammerText)) ButtonGrammer();
            if (GuiButton((Rectangle){ 360, 432, 96, 24 }, ButtonLanguageSoundText)) ButtonLanguageSound();
            if (GuiButton((Rectangle){ 672, 432, 96, 24 }, ButtonLanguageSoundExportText)) ButtonLanguageSoundExport();

            GuiLabel((Rectangle){ 0, 504, 512, 24 }, lexer_status);
            GuiLabel((Rectangle){ 0, 528, 512, 24 }, grammer_status);

            if (GuiDropdownBox((Rectangle){ 696, 168, 72, 24 }, DropDownBoxGeneratorWordSelectionText, &DropDownBoxGeneratorWordSelectionActive, DropDownBoxGeneratorWordSelectionEditMode)) DropDownBoxGeneratorWordSelectionEditMode = !DropDownBoxGeneratorWordSelectionEditMode;
            if (GuiDropdownBox((Rectangle){ 696, 48, 72, 24 }, DropDownBoxWordGeneratorTypeText, &DropDownBoxWordGeneratorTypeActive, DropDownBoxWordGeneratorTypeEditMode)) DropDownBoxWordGeneratorTypeEditMode = !DropDownBoxWordGeneratorTypeEditMode;
            if (GuiDropdownBox((Rectangle){ 104, 384, 88, 24 }, DropDownBoxVoiceNoiseTypeText, &DropDownBoxVoiceNoiseTypeActive, DropDownBoxVoiceNoiseTypeEditMode)) DropDownBoxVoiceNoiseTypeEditMode = !DropDownBoxVoiceNoiseTypeEditMode;
            if (GuiDropdownBox((Rectangle){ 528, 432, 120, 24 }, DropDownBoxLanguageSoundFormatText, &DropDownBoxLanguageSoundFormatActive, DropDownBoxLanguageSoundFormatEditMode)) {
                DropDownBoxLanguageSoundFormatEditMode = !DropDownBoxLanguageSoundFormatEditMode;
            }
            GuiUnlock();
            //----------------------------------------------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    audioDeinit();
    CloseAudioDevice();       // Close sound and sound context
    CloseWindow();            // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------
static void ButtonWordSearchEnglish()
{
    // TODO: Implement control logic
}
static void ButtonWordSearchLanguage()
{
    // TODO: Implement control logic
}
static void ButtonDictionaryUpdateEntry()
{
    // TODO: Implement control logic
}
static void ButtonDictionaryDeleteEntry()
{
    // TODO: Implement control logic
}
static void ButtonDictionaryPlaySound()
{
    // TODO: Implement control logic
}
static void ButtonWordGeneratorGenerate()
{
    const char fade_in[4][3] = {{'S','i','e'}, {'Q','i','e'}, {'T','i','e'}, {'W','i','e'}};
    const char fade_out[4][3] = {{'S','o','e'}, {'Q','o','e'}, {'T','o','e'}, {'W','o','e'}};
    const char normal[4][3] = {
        {'S','e','e'}, {'Q','e','e'}, {'T','e','e'}, {'W','e','e'}};
    const char pitch[8][3] = {
        {'S','e','l'}, {'Q','e','l'}, {'T','e','l'}, {'W','e','l'},
        {'S','e','h'}, {'Q','e','h'}, {'T','e','h'}, {'W','e','h'}};

    char word[5 * 3 + 1] = {0};

    if(GetRandomValue(0, 1))
        strncat(word, fade_in[GetRandomValue(0, sizeof(fade_in) / sizeof(fade_in[0]) - 1)], 3);
    else
        strncat(word, fade_out[GetRandomValue(0, sizeof(fade_out) / sizeof(fade_out[0]) - 1)], 3);

    const unsigned int which_pitch = GetRandomValue(0, 4);
    const unsigned int middle_notes = GetRandomValue(1, 3);

    for(unsigned int i = 0; i < middle_notes; i++) {
        if(which_pitch == i)
            strncat(word, pitch[GetRandomValue(0, sizeof(pitch) / sizeof(pitch[0]) - 1)], 3);
        else
            strncat(word, normal[GetRandomValue(0, sizeof(normal) / sizeof(normal[0]) - 1)], 3);
    }

    if(GetRandomValue(0, 1))
        strncat(word, fade_out[GetRandomValue(0, sizeof(fade_out) / sizeof(fade_out[0]) - 1)], 3);

    printf("Word: %s.\n", word);
    snprintf(TextBoxLanguageEntryText, sizeof(TextBoxLanguageEntryText) / sizeof(TextBoxLanguageEntryText[0]), "%s", word );
}
static void ButtonGeneratorWordReplace()
{
    // TODO: Implement control logic
}
static void ButtonVoiceNoiseTest()
{
    mtx_lock(&audio_context_mtx);

    audio_context.volume = ValueBoxVoiceVolumeValue;
    audio_context.min_frequency = ValueBoxVoiceFreqValue;
    audio_context.add_frequency = ValueBoxVoiceFreqPlusValue;

    voiceGenerateAllPhonemics(&audio_context.voice_context, audio_context.volume, audio_context.min_frequency, audio_context.add_frequency);

    voiceReadyContext(&audio_context.voice_context);

    mtx_unlock(&audio_context_mtx);

    audioPlay();
}
static void ButtonGrammer()
{
    grammerParse(TextBoxLanguageEntryText);
}
static void ButtonLanguageSound()
{
    unsigned int length = 0;

    if(TextBoxLanguageEntryText[0] != '\0') {
        char cleanup[128] = "";
        unsigned int n = 0;
        unsigned int c = 0;

        while(isspace(TextBoxLanguageEntryText[n]))
            n++;

        while(TextBoxLanguageEntryText[n] != '\0') {
            cleanup[length] = tolower(TextBoxLanguageEntryText[n]);

            if(c % 3 == 0)
                cleanup[length] = toupper(cleanup[length]);

            n++;

            if(isspace(TextBoxLanguageEntryText[n]))
                cleanup[++length] = ' ';

            while(isspace(TextBoxLanguageEntryText[n]))
                n++;

            if(cleanup[length] == ' ')
                c = 0;
            else
                c++;

            length++;
        }

        if(length != 0 && cleanup[length - 1] == ' ') {
            cleanup[length - 1] = '\0';
            length--;
        }

        for(length = 0; cleanup[length] != '\0'; length++)
            TextBoxLanguageEntryText[length] = cleanup[length];

        TextBoxLanguageEntryText[length] = cleanup[length];
    }

    mtx_lock(&audio_context_mtx);

    audio_context.voice_context.note_amount = 0;
    audio_context.voice_context.call_reloader = NULL;
    audio_context.head = 0;
    audio_context.volume = ValueBoxVoiceVolumeValue;
    audio_context.min_frequency = ValueBoxVoiceFreqValue;
    audio_context.add_frequency = ValueBoxVoiceFreqPlusValue;

    audio_context.length = length;
    audio_context.text = TextBoxLanguageEntryText;

    voiceFromTextSetup(&audio_context);

    mtx_unlock(&audio_context_mtx);

    audioPlay();
}

static void ButtonLanguageSoundExport()
{
    VoiceTextContext context;

    context.voice_context.note_amount = 0;
    context.voice_context.call_reloader = NULL;
    context.head = 0;
    context.volume = ValueBoxVoiceVolumeValue;
    context.min_frequency = ValueBoxVoiceFreqValue;
    context.add_frequency = ValueBoxVoiceFreqPlusValue;

    context.text = TextBoxLanguageEntryText;
    context.length = strlen(TextBoxLanguageEntryText);

    voiceFromTextSetup(&context);

    if(DropDownBoxLanguageSoundFormatActive == 0)
        voiceFromTextExportWAV(&context, "export.wav");
    else {
        FILE *file = fopen("export.txt", "w");

        if(file != NULL) {
            fputs(TextBoxLanguageEntryText, file);

            fclose(file);
        }
    }
}

