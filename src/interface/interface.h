#pragma once
#include "types/marshal/decompilationinfo.h"
#include "types/marshal/marshalexception.h"
#include "types/marshal/marshalexpected.h"
#include "types/native/assemblyinfo.h"

class Interface
{
public:
    static QString decompileType(TypeDefinitionHandle handle, const DecompilationInfo& info);
    static MarshalExpected<NativeTypes::AssemblyInfo, MarshalException> getAssembly(const QString& assemblyPath);
private:
    typedef MarshalString* (*fnDecompileType)(TypeDefinitionHandle handle, const DecompilationInfo& info);
    typedef MarshalExpected<MarshalTypes::AssemblyInfo, MarshalException>* (*fnGetAssembly)(const MarshalString& assemblyPath);
};
