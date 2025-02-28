namespace DotnetBinaryInterface.AssemblyTypes;

[StructLayout(LayoutKind.Sequential)]
public readonly struct AssemblyPEHeader(PEHeader peHeader)
{
    #region Standard fields
    public readonly PEMagic magic = peHeader.Magic;
    public readonly byte majorLinkerVersion = peHeader.MajorLinkerVersion;
    public readonly byte minorLinkerVersion = peHeader.MinorLinkerVersion;
    public readonly int sizeOfCode = peHeader.SizeOfCode;
    public readonly int sizeOfInitializedData = peHeader.SizeOfInitializedData;
    public readonly int sizeOfUninitializedData = peHeader.SizeOfUninitializedData;
    public readonly int addressOfEntryPoint = peHeader.AddressOfEntryPoint;
    public readonly int baseOfCode = peHeader.BaseOfCode;
    public readonly int baseOfData = peHeader.BaseOfData;
    #endregion

    #region Windows Specific Fields
    public readonly ulong imageBase = peHeader.ImageBase;
    public readonly int sectionAlignment = peHeader.SectionAlignment;
    public readonly int fileAlignment = peHeader.FileAlignment;
    public readonly ushort majorOperatingSystemVersion = peHeader.MajorOperatingSystemVersion;
    public readonly ushort minorOperatingSystemVersion = peHeader.MinorOperatingSystemVersion;
    public readonly ushort majorImageVersion = peHeader.MajorImageVersion;
    public readonly ushort minorImageVersion = peHeader.MinorImageVersion;
    public readonly ushort majorSubsystemVersion = peHeader.MajorSubsystemVersion;
    public readonly ushort minorSubsystemVersion = peHeader.MinorSubsystemVersion;
    public readonly int sizeOfImage = peHeader.SizeOfImage;
    public readonly int sizeOfHeaders = peHeader.SizeOfHeaders;
    public readonly uint checksum = peHeader.CheckSum;
    public readonly Subsystem subsystem = peHeader.Subsystem;
    public readonly DllCharacteristics dllCharacteristics = peHeader.DllCharacteristics;
    public readonly ulong sizeOfStackReserve = peHeader.SizeOfStackReserve;
    public readonly ulong sizeOfStackCommit = peHeader.SizeOfStackCommit;
    public readonly ulong sizeOfHeapReserve = peHeader.SizeOfHeapReserve;
    public readonly ulong sizeOfHeapCommit = peHeader.SizeOfHeapCommit;
    public readonly int numberOfRvaAndSizes = peHeader.NumberOfRvaAndSizes;
    #endregion

    #region Directory Entries
    public readonly DirectoryEntry exportTableDirectory = peHeader.ExportTableDirectory;
    public readonly DirectoryEntry importTableDirectory = peHeader.ImportTableDirectory;
    public readonly DirectoryEntry resourceTableDirectory = peHeader.ResourceTableDirectory;
    public readonly DirectoryEntry exceptionTableDirectory = peHeader.ExceptionTableDirectory;
    public readonly DirectoryEntry certificateTableDirectory = peHeader.CertificateTableDirectory;
    public readonly DirectoryEntry baseRelocationTableDirectory = peHeader.BaseRelocationTableDirectory;
    public readonly DirectoryEntry debugTableDirectory = peHeader.DebugTableDirectory;
    public readonly DirectoryEntry copyrightTableDirectory = peHeader.CopyrightTableDirectory;
    public readonly DirectoryEntry globalPointerTableDirectory = peHeader.GlobalPointerTableDirectory;
    public readonly DirectoryEntry threadLocalStorageTableDirectory = peHeader.ThreadLocalStorageTableDirectory;
    public readonly DirectoryEntry loadConfigTableDirectory = peHeader.LoadConfigTableDirectory;
    public readonly DirectoryEntry boundImportTableDirectory = peHeader.BoundImportTableDirectory;
    public readonly DirectoryEntry importAddressTableDirectory = peHeader.ImportAddressTableDirectory;
    public readonly DirectoryEntry delayImportTableDirectory = peHeader.DelayImportTableDirectory;
    public readonly DirectoryEntry corHeaderTableDirectory = peHeader.CorHeaderTableDirectory;
    #endregion
}