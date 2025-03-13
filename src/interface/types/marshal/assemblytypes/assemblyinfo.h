#pragma once
#include "assemblymetadata.h"
#include "assemblypeheaders.h"
#include "assemblytypemetadata.h"

namespace MarshalTypes
{
    struct AssemblyInfo
    {
        AssemblyMetadata metadata;
        AssemblyPEHeaders peHeaders;
        MarshalList<AssemblyMetadata> references;
        MarshalList<AssemblyTypeMetadata> types;
    };
}
