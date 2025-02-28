namespace DotnetBinaryInterface.AssemblyTypes;

[StructLayout(LayoutKind.Sequential)]
public readonly struct AssemblyCorHeader(CorHeader corHeader)
{
    public readonly ushort majorRuntimeVersion = corHeader.MajorRuntimeVersion;
    public readonly ushort minorRuntimeVersion = corHeader.MinorRuntimeVersion;
    public readonly DirectoryEntry metadataDirectory = corHeader.MetadataDirectory;
    public readonly CorFlags flags = corHeader.Flags;
    public readonly int entryPointTokenOrRelativeVirtualAddress = corHeader.EntryPointTokenOrRelativeVirtualAddress;
    public readonly DirectoryEntry resourcesDirectory = corHeader.ResourcesDirectory;
    public readonly DirectoryEntry strongNameSignatureDirectory = corHeader.StrongNameSignatureDirectory;
    public readonly DirectoryEntry codeManagerTableDirectory = corHeader.CodeManagerTableDirectory;
    public readonly DirectoryEntry vtableFixupsDirectory = corHeader.VtableFixupsDirectory;
    public readonly DirectoryEntry exportAddressTableJumpsDirectory = corHeader.ExportAddressTableJumpsDirectory;
    public readonly DirectoryEntry managedNativeHeaderDirectory = corHeader.ManagedNativeHeaderDirectory;
}