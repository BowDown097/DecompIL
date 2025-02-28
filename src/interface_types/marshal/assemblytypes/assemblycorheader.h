#pragma once
#include "shared.h"

namespace MarshalTypes
{
    struct AssemblyCorHeader
    {
        unsigned short majorRuntimeVersion;
        unsigned short minorRuntimeVersion;
        DirectoryEntry metadataDirectory;
        CorFlags flags;
        int entryPointTokenOrRelativeVirtualAddress;
        DirectoryEntry resourcesDirectory;
        DirectoryEntry strongNameSignatureDirectory;
        DirectoryEntry codeManagerTableDirectory;
        DirectoryEntry vtableFixupsDirectory;
        DirectoryEntry exportAddressTableJumpsDirectory;
        DirectoryEntry managedNativeHeaderDirectory;
    };
}
