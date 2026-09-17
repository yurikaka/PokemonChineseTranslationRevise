typedef unsigned short u16;
typedef unsigned int u32;

#include "../../../common/mode2_japanese_names.h"

typedef struct { u32 unused0, heapId, unused8; void *buffer; } Mode2StringTemplate;
extern u32 BoxPokemon_GetValue(void *boxMon, int attr, void *dest);
extern void SetStringTemplateArg(void *format, u32 field, void *string, void *grammar);
extern void StringTemplate_SetSpeciesName(void *format, u32 field, void *boxMon);
extern u32 Pokemon_GetValue(void *mon, int attr, void *dest);
extern void *GetSpeciesName(u16 species, int heapId);
extern void CopyStringToU16Array(void *string, u16 *dest, int length);
extern void StringDelete(void *string);
extern void StringCopy(void *dest, void *source);
extern void StringCopyChars(void *dest, const u16 *source);

void Mode2_CopyJapaneseSpeciesName_Impl(u16 species, u16 *dest)
{
    const u16 *src = sMode2JapaneseNames[species < 496 ? species : 0];
    int i;
    for (i = 0; i < 11; i++) dest[i] = src[i];
}

void Mode2_GetBattleDisplayNickname_Impl(void *mon, u16 *dest)
{
    if (Pokemon_GetValue(mon, 77, 0)) Pokemon_GetValue(mon, 117, dest);
    else {
        void *string = GetSpeciesName((u16)Pokemon_GetValue(mon, 5, 0), 0);
        CopyStringToU16Array(string, dest, 11);
        StringDelete(string);
    }
}

void Mode2_BufferBoxMonNickname_Impl(void *format_, u32 field, void *boxMon)
{
    Mode2StringTemplate *format = format_;
    if (BoxPokemon_GetValue(boxMon, 77, 0)) {
        BoxPokemon_GetValue(boxMon, 119, format->buffer);
        SetStringTemplateArg(format, field, format->buffer, 0);
    } else {
        StringTemplate_SetSpeciesName(format, field, boxMon);
    }
}

void Mode2_CopyDisplayNicknameString_Impl(u16 species, void *dest, const u16 *raw, u32 flags)
{
    if (flags >> 31) StringCopyChars(dest, raw);
    else { void *string = GetSpeciesName(species, 0); StringCopy(dest, string); StringDelete(string); }
}
