#pragma once
#include "marshallist.h"
#include "marshalstring.h"

enum class DisplayLanguage { CSharp, IL };

enum class LanguageVersion
{
    CSharp1 = 1,
    CSharp2 = 2,
    CSharp3 = 3,
    CSharp4 = 4,
    CSharp5 = 5,
    CSharp6 = 6,
    CSharp7 = 7,
    CSharp7_1 = 701,
    CSharp7_2 = 702,
    CSharp7_3 = 703,
    CSharp8_0 = 800,
    CSharp9_0 = 900,
    CSharp10_0 = 1000,
    CSharp11_0 = 1100,
    CSharp12_0 = 1200,
    Preview = 1100,
    Latest = 0x7FFFFFFF
};

struct DecompilationInfo
{
    MarshalString assemblyPath;
    LanguageVersion csVersion;
    DisplayLanguage language;
    MarshalList<MarshalString> referenceDirs;
};
