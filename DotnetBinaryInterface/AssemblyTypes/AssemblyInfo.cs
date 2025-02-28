namespace DotnetBinaryInterface.AssemblyTypes;

[StructLayout(LayoutKind.Sequential)]
public readonly struct AssemblyInfo
{
    public readonly AssemblyMetadata metadata;
    public readonly AssemblyPEHeaders peHeaders;
    public readonly MarshalList<AssemblyMetadata> references;
    public readonly MarshalList<AssemblyTypeMetadata> types;

    public AssemblyInfo(PEReader peReader)
    {
        MetadataReader metadataReader = peReader.GetMetadataReader();
        metadata = new AssemblyMetadata(metadataReader);
        peHeaders = new AssemblyPEHeaders(peReader.PEHeaders);

        foreach (AssemblyReferenceHandle asmRefHandle in metadataReader.AssemblyReferences)
            references.Add(new AssemblyMetadata(metadataReader, asmRefHandle));

        foreach (TypeDefinitionHandle typeHandle in metadataReader.TypeDefinitions)
        {
            TypeDefinition definition = metadataReader.GetTypeDefinition(typeHandle);
            if (!definition.IsNested) // nested types will be a part of their parent's nestedTypes member
                types.Add(new AssemblyTypeMetadata(metadataReader, typeHandle, in definition));
        }
    }
}