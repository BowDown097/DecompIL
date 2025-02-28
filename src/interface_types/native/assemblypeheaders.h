#pragma once
#include "assemblysectionheader.h"
#include "interface_types/marshal/assemblytypes/assemblypeheaders.h"

namespace NativeTypes
{
    struct AssemblyPEHeaders
    {
        MarshalTypes::AssemblyCoffHeader coffHeader;
        MarshalNullable<MarshalTypes::AssemblyCorHeader> corHeader;
        MarshalNullable<MarshalTypes::AssemblyPEHeader> peHeader;
        QList<AssemblySectionHeader> sectionHeaders;

        AssemblyPEHeaders(MarshalTypes::AssemblyPEHeaders&& other)
            : coffHeader(other.coffHeader),
              corHeader(std::move(other.corHeader)),
              peHeader(std::move(other.peHeader)),
              sectionHeaders(std::move(other.sectionHeaders)) {}
    };
}
