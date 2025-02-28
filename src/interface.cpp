#include "interface.h"
#include "libraryhandle.h"

LibraryHandle handle("./" DECOMPIL_BINARY_INTERFACE_NAME);

MarshalExpected<NativeTypes::AssemblyInfo, MarshalException> Interface::getAssembly(const QString& assemblyPath)
{
    auto getAssembly = fnGetAssembly(handle.resolveFunction("GetAssembly"));
    std::unique_ptr<MarshalExpected<MarshalTypes::AssemblyInfo, MarshalException>> res(getAssembly(assemblyPath));
    return std::move(*res);
}
