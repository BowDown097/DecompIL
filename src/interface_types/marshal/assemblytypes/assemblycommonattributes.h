#pragma once
#include "interface_types/marshal/marshalstring.h"

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
