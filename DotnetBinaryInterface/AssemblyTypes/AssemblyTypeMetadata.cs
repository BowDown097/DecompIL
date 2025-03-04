namespace DotnetBinaryInterface.AssemblyTypes;

[StructLayout(LayoutKind.Sequential)]
public readonly struct AssemblyTypeMetadata
{
    public enum Kind
    {
        Class, Delegate, Enum, Interface, Struct
    }

    public readonly TypeAttributes attributes;
    public readonly TypeDefinitionHandle handle;
    public readonly bool isCompilerGenerated;
    public readonly bool isReadOnly;
    public readonly Kind kind;
    public readonly MarshalString name;
    public readonly MarshalString namespaceName;
    public readonly MarshalList<AssemblyTypeMetadata> nestedTypes;

    public AssemblyTypeMetadata(
        MetadataReader metadataReader, TypeDefinitionHandle handle, ref readonly TypeDefinition definition)
    {
        this.handle = handle;

        attributes = definition.Attributes;
        name = new MarshalString(definition.Name, metadataReader);
        namespaceName = new MarshalString(definition.Namespace, metadataReader);

        if (definition.Attributes.HasFlag(TypeAttributes.Interface))
        {
            kind = Kind.Interface;
        }
        else
        {
            EntityHandle baseType = definition.GetBaseTypeOrNil();
            if (!baseType.IsNil)
            {
                if (baseType.IsKnownType(metadataReader, KnownTypeCode.MulticastDelegate))
                    kind = Kind.Delegate;
                else if (baseType.IsKnownType(metadataReader, KnownTypeCode.Enum))
                    kind = Kind.Enum;
                else if (baseType.IsKnownType(metadataReader, KnownTypeCode.ValueType))
                    kind = Kind.Struct;
                else if (baseType.IsKnownType(metadataReader, KnownTypeCode.Object))
                    kind = Kind.Class;
            }
        }

        foreach (CustomAttributeHandle attributeHandle in definition.GetCustomAttributes())
        {
            CustomAttribute attribute = metadataReader.GetCustomAttribute(attributeHandle);
            string attributeName = metadataReader.GetCustomAttributeName(attribute);
            if (attributeName == nameof(IsReadOnlyAttribute))
                isReadOnly = true;
            else if (attributeName == nameof(CompilerGeneratedAttribute) && IsCompilerGeneratedName(in name))
                isCompilerGenerated = true;
        }

        foreach (TypeDefinitionHandle nestedTypeHandle in definition.GetNestedTypes())
        {
            TypeDefinition nestedTypeDefinition = metadataReader.GetTypeDefinition(nestedTypeHandle);
            nestedTypes.Add(new AssemblyTypeMetadata(metadataReader, nestedTypeHandle, in nestedTypeDefinition));
        }
    }

    private static bool IsCompilerGeneratedName(ref readonly MarshalString name)
    {
        return name.Length() == 0 || name[0] == '<' || name.StartsWith("_Closure$__");
    }
}