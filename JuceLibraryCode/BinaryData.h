/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   bgfile_jpg;
    const int            bgfile_jpgSize = 1297149;

    extern const char*   Fantasy_obj;
    const int            Fantasy_objSize = 2317504;

    extern const char*   juce_icon_png;
    const int            juce_icon_pngSize = 105697;

    extern const char*   portmeirion_jpg;
    const int            portmeirion_jpgSize = 145904;

    extern const char*   teapot_obj;
    const int            teapot_objSize = 95000;

    extern const char*   tile_background_png;
    const int            tile_background_pngSize = 151;

    extern const char*   JuliafontRegular_ttf;
    const int            JuliafontRegular_ttfSize = 15516;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 7;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
