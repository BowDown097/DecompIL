namespace DotnetBinaryInterface;

internal static class MetadataReaderExt
{
    internal static string GetCustomAttributeName(this MetadataReader metadataReader, CustomAttribute attribute)
    {
        if (attribute.Constructor.Kind == HandleKind.MethodDefinition)
        {
            MethodDefinition mdef = metadataReader.GetMethodDefinition(
                (MethodDefinitionHandle)attribute.Constructor);
            TypeDefinition tdef = metadataReader.GetTypeDefinition(mdef.GetDeclaringType());
            return metadataReader.GetString(tdef.Name);
        }
        else if (attribute.Constructor.Kind == HandleKind.MemberReference)
        {
            MemberReference mref = metadataReader.GetMemberReference((MemberReferenceHandle)attribute.Constructor);
            if (mref.Parent.Kind == HandleKind.TypeReference)
            {
                TypeReference tref = metadataReader.GetTypeReference((TypeReferenceHandle)mref.Parent);
                return metadataReader.GetString(tref.Name);
            }
            else if (mref.Parent.Kind == HandleKind.TypeDefinition)
            {
                TypeDefinition tdef = metadataReader.GetTypeDefinition((TypeDefinitionHandle)mref.Parent);
                return metadataReader.GetString(tdef.Name);
            }
        }

        return "";
    }

    internal static string GetCustomAttributeValue(this MetadataReader metadataReader, CustomAttribute attribute)
    {
        // blobContent is a signature followed by a value
        // (in most cases, there are also ones that i think are just garbage data)
        // 3rd byte in the signature is the value length, in which the next byte is the value, BUT...
        // if that length byte is 128, then the real length is the 4th byte, and the next byte is the value.

        byte[] blobContent = [..metadataReader.GetBlobContent(attribute.Value)];
        if (blobContent.Length > 4 && blobContent[2] < blobContent.Length)
        {
            if (blobContent[2] == 128 && blobContent[3] < blobContent.Length)
                return TrimFromZero(Encoding.UTF8.GetString([..blobContent], 4, blobContent[3]));
            else
                return TrimFromZero(Encoding.UTF8.GetString([..blobContent], 3, blobContent[2]));
        }

        return "";
    }

    private static string TrimFromZero(string input)
    {
        int index = input.IndexOf('\u0000');
        return index > -1 ? input[..index] : input;
    }
}