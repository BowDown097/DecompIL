namespace DotnetBinaryInterface;

internal static class Utils
{
    internal static CSharpDecompiler GetDecompiler(
        string assemblyPath, ref readonly MarshalList<MarshalString> referenceDirs)
    {
        PEFile module = new(assemblyPath);
        UniversalAssemblyResolver resolver = new(assemblyPath, false, module.Metadata.DetectTargetFrameworkId());

        for (int i = 0; i < referenceDirs.Size(); i++)
            resolver.AddSearchDirectory(referenceDirs.At(i).ToString());

        return new CSharpDecompiler(assemblyPath, resolver, new DecompilerSettings {
            ApplyWindowsRuntimeProjections = true,
            ThrowOnAssemblyResolveErrors = false
        });
    }

    internal static unsafe T* StructureToPtr<T>([DisallowNull] T structure, bool fDeleteOld = false)
        where T : unmanaged
    {
        T* _ptr = (T*)Marshal.AllocHGlobal(sizeof(T));
        Marshal.StructureToPtr(structure, (nint)_ptr, fDeleteOld);
        return _ptr;
    }
}