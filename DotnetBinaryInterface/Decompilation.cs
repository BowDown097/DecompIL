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
    public static MarshalString* DecompileType(TypeDefinitionHandle handle, DecompilationInfo* info)
    {
        string? assemblyPath = info->assemblyPath.ToString()
            ?? throw new InvalidOperationException("Assembly path could not be resolved.");

        PEFile module = new(assemblyPath);
        UniversalAssemblyResolver resolver = new(assemblyPath, false, module.Metadata.DetectTargetFrameworkId());

        for (int i = 0; i < info->referenceDirs.Size(); i++)
            resolver.AddSearchDirectory(info->referenceDirs.At(i).ToString());

        switch (info->language)
        {
            case DisplayLanguage.CSharp:
            {
                CSharpDecompiler decompiler = new(module, resolver, new DecompilerSettings(info->csVersion) {
                    ApplyWindowsRuntimeProjections = true,
                    ThrowOnAssemblyResolveErrors = false
                });

                return Utils.StructureToPtr(new MarshalString(decompiler.DecompileAsString(handle)));
            }
            case DisplayLanguage.IL:
            {
                StringWriter sw = new();
                ReflectionDisassembler disassembler = new(new PlainTextOutput(sw), CancellationToken.None) {
                    AssemblyResolver = resolver
                };

                disassembler.DisassembleType(module, handle);
                return Utils.StructureToPtr(new MarshalString(sw.ToString()));
            }
            default: throw new InvalidOperationException("Invalid display language given.");
        }
    }

    [UnmanagedCallersOnly(EntryPoint = nameof(GetAssembly))]
    public static GetAssemblyResult* GetAssembly(MarshalString* assemblyPathIn)
    {
        try
        {
            string? assemblyPath = assemblyPathIn->ToString()
                ?? throw new InvalidOperationException("Assembly path could not be resolved.");

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