#pragma once
#include "interface/types/marshal/decompilationinfo.h"
#include <QHash>

static QHash<QString, LanguageVersion> CSVersionMap = {
    { "Latest", LanguageVersion::Latest },
    { "Preview", LanguageVersion::Preview },
    { "12.0 / VS 2022.8", LanguageVersion::CSharp12_0 },
    { "11.0 / VS 2022.4", LanguageVersion::CSharp11_0 },
    { "10.0 / VS 2022", LanguageVersion::CSharp10_0 },
    { "9.0 / VS 2019.8", LanguageVersion::CSharp9_0 },
    { "8.0 / VS 2019", LanguageVersion::CSharp8_0 },
    { "7.3 / VS 2017.7", LanguageVersion::CSharp7_3 },
    { "7.2 / VS 2017.4", LanguageVersion::CSharp7_2 },
    { "7.1 / VS 2017.3", LanguageVersion::CSharp7_1 },
    { "7.0 / VS 2017", LanguageVersion::CSharp7 },
    { "6.0 / VS 2015", LanguageVersion::CSharp6 },
    { "5.0 / VS 2012", LanguageVersion::CSharp5 },
    { "4.0 / VS 2010", LanguageVersion::CSharp4 },
    { "3.0 / VS 2008", LanguageVersion::CSharp3 },
    { "2.0 / VS 2005", LanguageVersion::CSharp2 },
    { "1.0 / VS .NET", LanguageVersion::CSharp1 }
};

static QHash<QString, DisplayLanguage> DisplayLanguageMap = {
    { "C#", DisplayLanguage::CSharp },
    { "IL", DisplayLanguage::IL }
};
