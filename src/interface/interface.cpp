#include "interface.h"
#include "libraryhandle.h"

LibraryHandle libHandle(DECOMPIL_BINARY_INTERFACE_NAME);

QString Interface::decompileType(const DecompilationInfo& info)
{
    auto decompileType = fnDecompileType(libHandle.resolveFunction("DecompileType"));
    std::unique_ptr<MarshalString> res(decompileType(info));
    return std::move(*res);
}

MarshalExpected<NativeTypes::AssemblyInfo, MarshalException> Interface::getAssembly(const QString& assemblyPath)
{
    auto getAssembly = fnGetAssembly(libHandle.resolveFunction("GetAssembly"));
    std::unique_ptr<MarshalExpected<MarshalTypes::AssemblyInfo, MarshalException>> res(getAssembly(assemblyPath));
    return std::move(*res);
}
