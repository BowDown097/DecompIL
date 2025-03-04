#pragma once
#include "interface_types/marshal/marshalexception.h"
#include "interface_types/marshal/marshalexpected.h"
#include "interface_types/native/assemblyinfo.h"

class Interface
{
public:
    static QString decompileType(const QString& assemblyPath, int handle);
    static MarshalExpected<NativeTypes::AssemblyInfo, MarshalException> getAssembly(const QString& assemblyPath);
private:
    typedef MarshalString* (*fnDecompileType)(const MarshalString&, int handle);
    typedef MarshalExpected<MarshalTypes::AssemblyInfo, MarshalException>* (*fnGetAssembly)(const MarshalString&);
};
