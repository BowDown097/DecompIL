#pragma once
#include "interface_types/marshal/assemblytypes/assemblycommonattributes.h"

namespace NativeTypes
{
    struct AssemblyCommonAttributes
    {
        QString company;
        QString copyright;
        QString informationalVersion;
        QString product;
        QString targetFramework;

        AssemblyCommonAttributes(const MarshalTypes::AssemblyCommonAttributes& other)
            : company(other.company),
              copyright(other.copyright),
              informationalVersion(other.informationalVersion),
              product(other.product),
              targetFramework(other.targetFramework) {}
    };
}
