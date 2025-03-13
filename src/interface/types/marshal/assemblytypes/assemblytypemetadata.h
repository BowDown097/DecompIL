#pragma once
#include "interface/types/marshal/marshallist.h"
#include "interface/types/marshal/marshalstring.h"
#include "shared.h"

enum class TypeKind { Class, Delegate, Enum, Interface, Struct };
using TypeDefinitionHandle = int;

namespace MarshalTypes
{
    struct AssemblyTypeMetadata
    {
        TypeAttributes attributes;
        TypeDefinitionHandle handle;
        bool isCompilerGenerated;
        bool isReadOnly;
        TypeKind kind;
        MarshalString name;
        MarshalString namespaceName;
        MarshalList<AssemblyTypeMetadata> nestedTypes;
    };
}
