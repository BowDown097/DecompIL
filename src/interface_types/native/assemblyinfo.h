#pragma once
#include "assemblymetadata.h"
#include "assemblypeheaders.h"
#include "assemblytypemetadata.h"
#include "interface_types/marshal/assemblytypes/assemblyinfo.h"

namespace NativeTypes
{
    struct AssemblyInfo
    {
        AssemblyMetadata metadata;
        AssemblyPEHeaders peHeaders;
        QList<AssemblyMetadata> references;
        QList<AssemblyTypeMetadata> types;

        AssemblyInfo(MarshalTypes::AssemblyInfo&& other)
            : metadata(other.metadata),
              peHeaders(std::move(other.peHeaders)),
              references(std::move(other.references)),
              types(std::move(other.types)) {}
    };
}
