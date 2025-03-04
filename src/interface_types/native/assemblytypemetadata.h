#pragma once
#include "interface_types/marshal/assemblytypes/assemblytypemetadata.h"

namespace NativeTypes
{
    struct AssemblyTypeMetadata
    {
        TypeAttributes attributes;
        int handle;
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
    };
}
