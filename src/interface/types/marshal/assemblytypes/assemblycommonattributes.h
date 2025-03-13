#pragma once
#include "interface/types/marshal/marshalstring.h"

namespace MarshalTypes
{
    struct AssemblyCommonAttributes
    {
        MarshalString company;
        MarshalString copyright;
        MarshalString informationalVersion;
        MarshalString product;
        MarshalString targetFramework;
    };
}
