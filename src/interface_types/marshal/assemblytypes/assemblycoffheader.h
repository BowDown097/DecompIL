#pragma once
#include "shared.h"

namespace MarshalTypes
{
    struct AssemblyCoffHeader
    {
        Machine machine;
        short numberOfSections;
        int timeDateStamp;
        int pointerToSymbolTable;
        int numberOfSymbols;
        short sizeOfOptionalHeader;
        Characteristics characteristics;
    };
}
