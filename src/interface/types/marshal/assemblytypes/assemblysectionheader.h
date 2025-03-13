#pragma once
#include "interface/types/marshal/marshalstring.h"
#include "shared.h"

namespace MarshalTypes
{
    struct AssemblySectionHeader
    {
        MarshalString name;
        int virtualSize;
        int virtualAddress;
        int sizeOfRawData;
        int pointerToRawData;
        int pointerToRelocations;
        int pointerToLineNumbers;
        unsigned short numberOfRelocations;
        unsigned short numberOfLineNumbers;
        SectionCharacteristics sectionCharacteristics;
    };
}
