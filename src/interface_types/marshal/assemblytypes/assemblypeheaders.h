#pragma once
#include "assemblycoffheader.h"
#include "assemblycorheader.h"
#include "assemblypeheader.h"
#include "assemblysectionheader.h"
#include "interface_types/marshal/marshallist.h"
#include "interface_types/marshal/marshalnullable.h"

namespace MarshalTypes
{
    struct AssemblyPEHeaders
    {
        AssemblyCoffHeader coffHeader;
        MarshalNullable<AssemblyCorHeader> corHeader;
        MarshalNullable<AssemblyPEHeader> peHeader;
        MarshalList<AssemblySectionHeader> sectionHeaders;
    };
}
