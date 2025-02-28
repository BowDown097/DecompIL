#pragma once
#include "shared.h"

namespace MarshalTypes
{
    struct AssemblyPEHeader
    {
        // Standard fields
        PEMagic magic;
        unsigned char majorLinkerVersion;
        unsigned char minorLinkerVersion;
        int sizeOfCode;
        int sizeOfInitializedData;
        int sizeOfUninitializedData;
        int addressOfEntryPoint;
        int baseOfCode;
        int baseOfData;

        // Windows Specific Fields
        unsigned long imageBase;
        int sectionAlignment;
        int fileAlignment;
        unsigned short majorOperatingSystemVersion;
        unsigned short minorOperatingSystemVersion;
        unsigned short majorImageVersion;
        unsigned short minorImageVersion;
        unsigned short majorSubsystemVersion;
        unsigned short minorSubsystemVersion;
        int sizeOfImage;
        int sizeOfHeaders;
        unsigned int checksum;
        Subsystem subsystem;
        DllCharacteristics dllCharacteristics;
        unsigned long sizeOfStackReserve;
        unsigned long sizeOfStackCommit;
        unsigned long sizeOfHeapReserve;
        unsigned long sizeOfHeapCommit;
        int numberOfRvaAndSizes;

        // Directory Entries
        DirectoryEntry exportTableDirectory;
        DirectoryEntry importTableDirectory;
        DirectoryEntry resourceTableDirectory;
        DirectoryEntry exceptionTableDirectory;
        DirectoryEntry certificateTableDirectory;
        DirectoryEntry baseRelocationTableDirectory;
        DirectoryEntry debugTableDirectory;
        DirectoryEntry copyrightTableDirectory;
        DirectoryEntry globalPointerTableDirectory;
        DirectoryEntry threadLocalStorageTableDirectory;
        DirectoryEntry loadConfigTableDirectory;
        DirectoryEntry boundImportTableDirectory;
        DirectoryEntry importAddressTableDirectory;
        DirectoryEntry delayImportTableDirectory;
        DirectoryEntry corHeaderTableDirectory;
    };
}
