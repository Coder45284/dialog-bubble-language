#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "constants.h"
#include "voice.h"

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

AudioStream voice;

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

    voice = LoadAudioStream(PCM_SAMPLES_PER_SECOND, PCM_SAMPLE_BITS, 1);

    SetAudioStreamCallback(voice, voiceSoundCallback);

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
    const char *DropDownBoxLanguageSoundFormatText = "WAV;FLAC;OGG;VOA;TRAC";
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
    int DropDownBoxLanguageSoundFormatActive = 0;
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

            if (GuiDropdownBox((Rectangle){ 696, 168, 72, 24 }, DropDownBoxGeneratorWordSelectionText, &DropDownBoxGeneratorWordSelectionActive, DropDownBoxGeneratorWordSelectionEditMode)) DropDownBoxGeneratorWordSelectionEditMode = !DropDownBoxGeneratorWordSelectionEditMode;
            if (GuiDropdownBox((Rectangle){ 696, 48, 72, 24 }, DropDownBoxWordGeneratorTypeText, &DropDownBoxWordGeneratorTypeActive, DropDownBoxWordGeneratorTypeEditMode)) DropDownBoxWordGeneratorTypeEditMode = !DropDownBoxWordGeneratorTypeEditMode;
            if (GuiDropdownBox((Rectangle){ 104, 384, 88, 24 }, DropDownBoxVoiceNoiseTypeText, &DropDownBoxVoiceNoiseTypeActive, DropDownBoxVoiceNoiseTypeEditMode)) DropDownBoxVoiceNoiseTypeEditMode = !DropDownBoxVoiceNoiseTypeEditMode;
            if (GuiDropdownBox((Rectangle){ 528, 432, 120, 24 }, DropDownBoxLanguageSoundFormatText, &DropDownBoxLanguageSoundFormatActive, DropDownBoxLanguageSoundFormatEditMode)) DropDownBoxLanguageSoundFormatEditMode = !DropDownBoxLanguageSoundFormatEditMode;
            
            GuiUnlock();
            //----------------------------------------------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadAudioStream(voice); // Unload the voice data.
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
    voiceContext.note_amount = 0;

    const char person[] = {'s','q','t','w'};
    const char gender[] = {'s','q', '\0'};
    const char  owner[] = {'s','t'};
    const char plural[] = {'e','h'};
    char single_phonem[4] = {'\0', '\0', '\0', '\0'};

    for(int p = 0; p < 4; p++) {
        for(int g = 0; g < 3; g++) {
            for(int o = 0; o < 2; o++) {
                for(int r = 0; r < 2; r++) {
                    // 's' is 1st person, 'q' is 2nd person, 't' is 3rd person for people, 'w' is 3rd person for objects
                    single_phonem[0] = person[p];
                    single_phonem[1] = 'i';
                    single_phonem[2] = 'h';
                    voiceInputPhonemic(&voiceContext, single_phonem, ValueBoxVoiceVolumeValue, ValueBoxVoiceFreqValue, ValueBoxVoiceFreqPlusValue);

                    if(gender[g] != '\0') {
                        single_phonem[0] = gender[g];
                        single_phonem[1] = 'e';
                        single_phonem[2] = 'e';
                        voiceInputPhonemic(&voiceContext, single_phonem, ValueBoxVoiceVolumeValue, ValueBoxVoiceFreqValue, ValueBoxVoiceFreqPlusValue);
                    }

                    single_phonem[0] = owner[o];
                    single_phonem[1] = 'o';
                    single_phonem[2] = plural[r];
                    voiceInputPhonemic(&voiceContext, single_phonem, ValueBoxVoiceVolumeValue, ValueBoxVoiceFreqValue, ValueBoxVoiceFreqPlusValue);

                    // Insert space
                    voiceInputPhonemic(&voiceContext, "", ValueBoxVoiceVolumeValue, ValueBoxVoiceFreqValue, ValueBoxVoiceFreqPlusValue);

                    if(gender[g] == '\0')
                        voiceInputPhonemic(&voiceContext, "", ValueBoxVoiceVolumeValue, ValueBoxVoiceFreqValue, ValueBoxVoiceFreqPlusValue);
                }
            }
        }
    }

    voiceReadyContext(&voiceContext);

    PlayAudioStream(voice);
}
static void ButtonWordGeneratorGenerate()
{
    // TODO: Implement control logic
}
static void ButtonGeneratorWordReplace()
{
    // TODO: Implement control logic
}
static void ButtonVoiceNoiseTest()
{
    voiceGenerateAllPhonemics(&voiceContext, ValueBoxVoiceVolumeValue, ValueBoxVoiceFreqValue, ValueBoxVoiceFreqPlusValue);

    voiceReadyContext(&voiceContext);

    PlayAudioStream(voice);
}
static void ButtonGrammer()
{
    // TODO: Implement control logic
}
static void ButtonLanguageSound()
{
    if(TextBoxLanguageEntryText[0] != '\0') {
        char cleanup[128] = "";
        unsigned int n = 0;
        unsigned int m = 0;

        while(isspace(TextBoxLanguageEntryText[n]))
            n++;

        while(TextBoxLanguageEntryText[n] != '\0') {
            cleanup[m] = tolower(TextBoxLanguageEntryText[n]);

            n++;

            if(isspace(TextBoxLanguageEntryText[n]))
                cleanup[++m] = ' ';

            while(isspace(TextBoxLanguageEntryText[n]))
                n++;

            m++;
        }

        if(m != 0 && cleanup[m - 1] == ' ') {
            cleanup[m - 1] = '\0';
            m--;
        }

        for(m = 0; cleanup[m] != '\0'; m++) {
            TextBoxLanguageEntryText[m] = cleanup[m];
        }

        TextBoxLanguageEntryText[m] = cleanup[m];
    }

    voiceContext.note_amount = 0;

    voiceReadyContext(&voiceContext);

    PlayAudioStream(voice);
}
static void ButtonLanguageSoundExport()
{
    VoiceContext context;

    voiceGenerateAllPhonemics(&context, ValueBoxVoiceVolumeValue, ValueBoxVoiceFreqValue, ValueBoxVoiceFreqPlusValue);

    voiceReadyContext(&context);

    voiceExportWAV("export.wav", context.note_amount, context.notes);
}

