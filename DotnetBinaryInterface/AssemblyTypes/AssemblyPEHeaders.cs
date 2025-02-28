namespace DotnetBinaryInterface.AssemblyTypes;

[StructLayout(LayoutKind.Sequential)]
public readonly struct AssemblyPEHeaders
{
    public readonly AssemblyCoffHeader coffHeader;
    public readonly MarshalNullable<AssemblyCorHeader> corHeader;
    public readonly MarshalNullable<AssemblyPEHeader> peHeader;
    public readonly MarshalList<AssemblySectionHeader> sectionHeaders;

    public AssemblyPEHeaders(PEHeaders peHeaders)
    {
        coffHeader = new AssemblyCoffHeader(peHeaders.CoffHeader);
        if (peHeaders.CorHeader is not null)
            corHeader = new AssemblyCorHeader(peHeaders.CorHeader);
        if (peHeaders.PEHeader is not null)
            peHeader = new AssemblyPEHeader(peHeaders.PEHeader);

        sectionHeaders.Reserve(peHeaders.SectionHeaders.Length);
        foreach (System.Reflection.PortableExecutable.SectionHeader sectionHeader in peHeaders.SectionHeaders)
            sectionHeaders.Add(new AssemblySectionHeader(in sectionHeader));
    }
}