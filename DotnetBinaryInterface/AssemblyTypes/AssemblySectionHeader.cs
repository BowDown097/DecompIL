namespace DotnetBinaryInterface.AssemblyTypes;

[StructLayout(LayoutKind.Sequential)]
public readonly struct AssemblySectionHeader(
    ref readonly System.Reflection.PortableExecutable.SectionHeader sectionHeader)
{
    public readonly MarshalString name = new(sectionHeader.Name);
    public readonly int virtualSize = sectionHeader.VirtualSize;
    public readonly int virtualAddress = sectionHeader.VirtualAddress;
    public readonly int sizeOfRawData = sectionHeader.SizeOfRawData;
    public readonly int pointerToRawData = sectionHeader.PointerToRawData;
    public readonly int pointerToRelocations = sectionHeader.PointerToRelocations;
    public readonly int pointerToLineNumbers = sectionHeader.PointerToLineNumbers;
    public readonly ushort numberOfRelocations = sectionHeader.NumberOfRelocations;
    public readonly ushort numberOfLineNumbers = sectionHeader.NumberOfLineNumbers;
    public readonly SectionCharacteristics sectionCharacteristics = sectionHeader.SectionCharacteristics;
}