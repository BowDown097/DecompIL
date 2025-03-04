namespace DotnetBinaryInterface.AssemblyTypes;

[StructLayout(LayoutKind.Sequential)]
public readonly struct AssemblyMetadata
{
    public readonly MarshalString name;
    public readonly MarshalString version;
    public readonly MarshalString culture;
    public readonly MarshalString publicKeyToken;
    public readonly AssemblyCommonAttributes commonAttributes;

    public AssemblyMetadata(MetadataReader metadataReader)
    {
        AssemblyDefinition assemblyDefinition = metadataReader.GetAssemblyDefinition();

        name = new MarshalString(assemblyDefinition.Name, metadataReader);
        version = new MarshalString(assemblyDefinition.Version.ToString());
        culture = new MarshalString(assemblyDefinition.Culture, metadataReader, "neutral");
        publicKeyToken = new MarshalString(CalculatePublicKeyToken(assemblyDefinition.PublicKey, metadataReader));
        commonAttributes = new AssemblyCommonAttributes(metadataReader, metadataReader.CustomAttributes);
    }

    public AssemblyMetadata(MetadataReader metadataReader, AssemblyReferenceHandle asmRefHandle)
    {
        if (asmRefHandle.IsNil)
            return;

        System.Reflection.Metadata.AssemblyReference reference = metadataReader.GetAssemblyReference(asmRefHandle);

        name = new MarshalString(reference.Name, metadataReader);
        version = new MarshalString(reference.Version.ToString());
        culture = new MarshalString(reference.Culture, metadataReader, "neutral");
        publicKeyToken = new MarshalString(CalculatePublicKeyToken(reference.PublicKeyOrToken, metadataReader));
        // no common attributes for references
    }

    private static string CalculatePublicKeyToken(BlobHandle blob, MetadataReader reader)
    {
        if (blob.IsNil)
            return "null";

        // Calculate public key token:
        // 1. hash the public key (always use SHA1).
        byte[] publicKeyTokenBytes = SHA1.HashData(reader.GetBlobBytes(blob));
        // 2. take the last 8 bytes
        // 3. according to Cecil we need to reverse them, other sources did not mention this.
        return publicKeyTokenBytes.TakeLast(8).Reverse().ToHexString(8);
    }
}