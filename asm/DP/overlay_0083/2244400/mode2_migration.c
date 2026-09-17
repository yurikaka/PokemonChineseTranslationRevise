typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int bool;

#include "../../../common/mode2_japanese_names.h"
#include "../../../common/mode2_english_names.h"

extern u32 GBABoxPokemon_GetData(void *gbaBoxMon, int attr, void *dest);
extern u32 GBAPokemon_ConvertSpeciesToDS(u32 species);
extern bool ConvertRSStringToDPStringInternational(const u8 *src, u16 *dest, u32 length, u32 language);
extern void BoxPokemon_SetValue(void *boxMon, int attr, const void *value);

static bool Mode2_NameEquals(const u16 *a, const u16 *b)
{
    int i;
    for (i = 0; i < 11; i++) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0xFFFF) return 1;
    }
    return 1;
}

void Mode2_SetMigratedNickname_Impl(void *gbaBoxMon, void *boxMon, u32 language)
{
    u8 raw[12];
    u16 decoded[12];
    u32 species;
    u32 hasNickname = 1;

    raw[10] = raw[11] = 0xFF;
    GBABoxPokemon_GetData(gbaBoxMon, 2, raw);
    ConvertRSStringToDPStringInternational(raw, decoded, 12, language);
    species = GBAPokemon_ConvertSpeciesToDS(GBABoxPokemon_GetData(gbaBoxMon, 11, 0));
    if (species <= 386) {
        if (language == 1 && Mode2_NameEquals(decoded, sMode2JapaneseNames[species])) hasNickname = 0;
        if (language == 2 && Mode2_NameEquals(decoded, sMode2EnglishNames[species])) hasNickname = 0;
    }
    BoxPokemon_SetValue(boxMon, 116, decoded);
    BoxPokemon_SetValue(boxMon, 77, &hasNickname);
}
