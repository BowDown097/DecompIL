#pragma once
#include "assemblycommonattributes.h"

namespace MarshalTypes
{
    struct AssemblyMetadata
    {
        MarshalString name;
        MarshalString version;
        MarshalString culture;
        MarshalString publicKeyToken;
        AssemblyCommonAttributes commonAttributes;
    };
}
