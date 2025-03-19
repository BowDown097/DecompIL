namespace DotnetBinaryInterface;

using GetAssemblyResult = MarshalExpected<AssemblyInfo, MarshalException>;

public static unsafe class Decompilation
{
    [UnmanagedCallersOnly(EntryPoint = nameof(DecompileType))]
    public static MarshalString* DecompileType(DecompilationInfo* info)
    {
        string? assemblyPath = info->assemblyPath.ToString()
            ?? throw new InvalidOperationException("Assembly path could not be resolved.");

        PEFile module = new(assemblyPath);
        UniversalAssemblyResolver resolver = new(assemblyPath, false, module.Metadata.DetectTargetFrameworkId());

        for (int i = 0; i < info->probingPaths.Size(); i++)
            resolver.AddSearchDirectory(info->probingPaths[i].ToString());

        switch (info->language)
        {
            case DisplayLanguage.CSharp:
            {
                CSharpDecompiler decompiler = new(module, resolver, new DecompilerSettings(info->csVersion) {
                    ApplyWindowsRuntimeProjections = true,
                    ThrowOnAssemblyResolveErrors = false
                });

                return Utils.StructureToPtr(new MarshalString(decompiler.DecompileAsString(info->handle)));
            }
            case DisplayLanguage.IL:
            {
                StringWriter sw = new();
                ReflectionDisassembler disassembler = new(new PlainTextOutput(sw), CancellationToken.None) {
                    AssemblyResolver = resolver
                };

                disassembler.DisassembleType(module, info->handle);
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

            return Utils.StructureToPtr<GetAssemblyResult>(new AssemblyInfo(peReader));
        }
        catch (Exception e)
        {
            return Utils.StructureToPtr<GetAssemblyResult>(new MarshalException(e));
        }
    }
}