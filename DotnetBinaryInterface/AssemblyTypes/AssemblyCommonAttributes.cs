namespace DotnetBinaryInterface.AssemblyTypes;

[StructLayout(LayoutKind.Sequential)]
public readonly struct AssemblyCommonAttributes
{
    public readonly MarshalString company;
    public readonly MarshalString copyright;
    public readonly MarshalString informationalVersion;
    public readonly MarshalString product;
    public readonly MarshalString targetFramework;

    public AssemblyCommonAttributes(MetadataReader metadataReader, CustomAttributeHandleCollection attributeHandles)
    {
        foreach (CustomAttributeHandle attributeHandle in attributeHandles)
        {
            if (attributeHandle.IsNil)
                continue;

            CustomAttribute attribute = metadataReader.GetCustomAttribute(attributeHandle);
            string attributeName = metadataReader.GetCustomAttributeName(attribute);

            switch (attributeName)
            {
                case nameof(AssemblyCompanyAttribute):
                    company = new MarshalString(metadataReader.GetCustomAttributeValue(attribute));
                    break;
                case nameof(AssemblyProductAttribute):
                    product = new MarshalString(metadataReader.GetCustomAttributeValue(attribute));
                    break;
                case nameof(AssemblyCopyrightAttribute):
                    copyright = new MarshalString(metadataReader.GetCustomAttributeValue(attribute));
                    break;
                case nameof(AssemblyInformationalVersionAttribute):
                    informationalVersion = new MarshalString(metadataReader.GetCustomAttributeValue(attribute));
                    break;
                case nameof(TargetFrameworkAttribute):
                    targetFramework = new MarshalString(metadataReader.GetCustomAttributeValue(attribute));
                    break;
            }
        }
    }
}