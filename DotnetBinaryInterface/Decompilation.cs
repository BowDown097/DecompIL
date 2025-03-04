using ICSharpCode.Decompiler;
using ICSharpCode.Decompiler.CSharp;

namespace DotnetBinaryInterface;

// we need to make a wrapper struct because StructureToPtr
// does not accept generic structs but will accept
// wrappers around them just fine. wonderful.
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public readonly struct GetAssemblyResult(MarshalExpected<AssemblyInfo, MarshalException> data)
{
    public readonly MarshalExpected<AssemblyInfo, MarshalException> _data = data;
}

public static unsafe class Decompilation
{
    [UnmanagedCallersOnly(EntryPoint = nameof(DecompileType))]
    public static MarshalString* DecompileType(MarshalString* assemblyPathIn, TypeDefinitionHandle handle)
    {
        string? assemblyPath = assemblyPathIn->ToString();
        if (assemblyPath is null)
            return (MarshalString*)IntPtr.Zero;

        CSharpDecompiler decompiler = new(assemblyPath, new DecompilerSettings {
            ApplyWindowsRuntimeProjections = true,
            ThrowOnAssemblyResolveErrors = false
        });

        return Utils.StructureToPtr(new MarshalString(decompiler.DecompileAsString(handle)));
    }

    [UnmanagedCallersOnly(EntryPoint = nameof(GetAssembly))]
    public static GetAssemblyResult* GetAssembly(MarshalString* assemblyPathIn)
    {
        try
        {
            string? assemblyPath = assemblyPathIn->ToString();
            if (assemblyPath is null)
                return (GetAssemblyResult*)IntPtr.Zero;

            using FileStream assemblyStream = File.OpenRead(assemblyPath);
            using PEReader peReader = new(assemblyStream);
            return Utils.StructureToPtr(new GetAssemblyResult(new AssemblyInfo(peReader)));
        }
        catch (Exception e)
        {
            return Utils.StructureToPtr(new GetAssemblyResult(new MarshalException(e)));
        }
    }
}