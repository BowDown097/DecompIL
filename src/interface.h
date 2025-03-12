#pragma once
#include "interface_types/marshal/decompilationinfo.h"
#include "interface_types/marshal/marshalexception.h"
#include "interface_types/marshal/marshalexpected.h"
#include "interface_types/native/assemblyinfo.h"

class Interface
{
public:
    static QString decompileType(TypeDefinitionHandle handle, const DecompilationInfo& info);
    static MarshalExpected<NativeTypes::AssemblyInfo, MarshalException> getAssembly(const QString& assemblyPath);
private:
    typedef MarshalString* (*fnDecompileType)(TypeDefinitionHandle handle, const DecompilationInfo& info);
    typedef MarshalExpected<MarshalTypes::AssemblyInfo, MarshalException>* (*fnGetAssembly)(const MarshalString& assemblyPath);
};
