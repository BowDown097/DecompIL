#pragma once
#include "interface_types/marshal/marshalexception.h"
#include "interface_types/marshal/marshalexpected.h"
#include "interface_types/native/assemblyinfo.h"

class Interface
{
public:
    static MarshalExpected<NativeTypes::AssemblyInfo, MarshalException> getAssembly(const QString& assemblyPath);
private:
    typedef MarshalExpected<MarshalTypes::AssemblyInfo, MarshalException>* (*fnGetAssembly)(const MarshalString&);
};
