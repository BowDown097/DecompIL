namespace DotnetBinaryInterface.AssemblyTypes;

[StructLayout(LayoutKind.Sequential)]
public readonly struct AssemblyCoffHeader(CoffHeader coffHeader)
{
    public readonly Machine machine = coffHeader.Machine;
    public readonly short numberOfSections = coffHeader.NumberOfSections;
    public readonly int timeDateStamp = coffHeader.TimeDateStamp;
    public readonly int pointerToSymbolTable = coffHeader.PointerToSymbolTable;
    public readonly int numberOfSymbols = coffHeader.NumberOfSymbols;
    public readonly short sizeOfOptionalHeader = coffHeader.SizeOfOptionalHeader;
    public readonly Characteristics characteristics = coffHeader.Characteristics;
}