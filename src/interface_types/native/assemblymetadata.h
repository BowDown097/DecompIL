#pragma once
#include "assemblycommonattributes.h"
#include "interface_types/marshal/assemblytypes/assemblymetadata.h"

namespace NativeTypes
{
    struct AssemblyMetadata
    {
        QString name;
        QString version;
        QString culture;
        QString publicKeyToken;
        AssemblyCommonAttributes commonAttributes;

        AssemblyMetadata(const MarshalTypes::AssemblyMetadata& other)
            : name(other.name),
              version(other.version),
              culture(other.culture),
              publicKeyToken(other.publicKeyToken),
              commonAttributes(other.commonAttributes) {}
    };
}
