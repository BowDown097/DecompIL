#pragma once
#include "interface/types/marshal/assemblytypes/assemblytypemetadata.h"

namespace NativeTypes
{
    struct AssemblyTypeMetadata
    {
        TypeAttributes attributes;
        TypeDefinitionHandle handle;
        bool isCompilerGenerated;
        bool isReadOnly;
        TypeKind kind;
        QString name;
        QString namespaceName;
        QList<AssemblyTypeMetadata> nestedTypes;

        AssemblyTypeMetadata(MarshalTypes::AssemblyTypeMetadata&& other)
            : attributes(other.attributes),
              handle(other.handle),
              isCompilerGenerated(other.isCompilerGenerated),
              isReadOnly(other.isReadOnly),
              kind(other.kind),
              name(other.name),
              namespaceName(other.namespaceName),
              nestedTypes(std::move(other.nestedTypes)) {}

        bool isStatic() const
        {
            return enum_ops::has_flag(attributes, TypeAttributes::Abstract) &&
                   enum_ops::has_flag(attributes, TypeAttributes::Sealed);
        }

        friend std::strong_ordering operator<=>(
            const NativeTypes::AssemblyTypeMetadata& lhs, const NativeTypes::AssemblyTypeMetadata& rhs)
        {
            if (std::strong_ordering cmp = lhs.namespaceName <=> rhs.namespaceName; cmp != 0) return cmp;
            return lhs.name <=> rhs.name;
        }

        friend std::strong_ordering operator<=>(
            const NativeTypes::AssemblyTypeMetadata& meta, const QString& name)
        {
            return meta.name <=> name;
        }
    };
}
