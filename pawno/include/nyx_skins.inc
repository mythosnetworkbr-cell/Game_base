#if defined _NYX_SKINS_INCLUDED
    #endinput
#endif
#define _NYX_SKINS_INCLUDED

#define NYX_SKIN_COUNT 40

// Presets utilizando IDs de skins nativas do GTA San Andreas/SA-MP.
new const NYX_SkinId[NYX_SKIN_COUNT] = {
    23, 93, 7, 46, 60, 98, 101, 102, 103, 104,
    105, 106, 107, 108, 109, 110, 111, 112, 113, 114,
    115, 116, 117, 118, 120, 121, 122, 123, 124, 125,
    126, 127, 128, 129, 130, 131, 132, 133, 280, 281
};

new const NYX_SkinName[NYX_SKIN_COUNT][] = {
    "Civil 01", "Civil 02", "Civil 03", "Civil 04", "Civil 05", "Civil 06", "Civil 07", "Policial 01",
    "Policial 02", "Policial 03", "Policial 04", "Policial 05", "Policial 06", "Policial 07", "Policial 08", "Policial 09",
    "Policial 10", "Policial 11", "Policial 12", "Policial 13", "Policial 14", "Policial 15", "Policial 16", "Policial 17",
    "Civil 08", "Civil 09", "Civil 10", "Civil 11", "Civil 12", "Civil 13", "Civil 14", "Civil 15",
    "Civil 16", "Civil 17", "Civil 18", "Civil 19", "Civil 20", "Civil 21", "Policial Elite 01", "Policial Elite 02"
};

stock NYX_GetSkin(index)
{
    if (index < 0 || index >= NYX_SKIN_COUNT) return 23;
    return NYX_SkinId[index];
}
