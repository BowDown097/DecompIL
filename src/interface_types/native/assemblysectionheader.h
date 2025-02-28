#pragma once
#include "interface_types/marshal/assemblytypes/assemblysectionheader.h"

namespace NativeTypes
{
    struct AssemblySectionHeader
    {
        QString name;
        int virtualSize;
        int virtualAddress;
        int sizeOfRawData;
        int pointerToRawData;
        int pointerToRelocations;
        int pointerToLineNumbers;
        unsigned short numberOfRelocations;
        unsigned short numberOfLineNumbers;
        SectionCharacteristics sectionCharacteristics;

        AssemblySectionHeader(const MarshalTypes::AssemblySectionHeader& other)
            : name(other.name),
              virtualSize(other.virtualSize),
              virtualAddress(other.virtualAddress),
              sizeOfRawData(other.sizeOfRawData),
              pointerToRawData(other.pointerToRawData),
              pointerToRelocations(other.pointerToRelocations),
              pointerToLineNumbers(other.pointerToLineNumbers),
              numberOfRelocations(other.numberOfRelocations),
              numberOfLineNumbers(other.numberOfLineNumbers),
              sectionCharacteristics(other.sectionCharacteristics) {}
    };
}
