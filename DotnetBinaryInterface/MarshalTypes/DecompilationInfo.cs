namespace DotnetBinaryInterface.MarshalTypes;

public enum DisplayLanguage { CSharp, IL };

[StructLayout(LayoutKind.Sequential)]
public readonly struct DecompilationInfo
{
    public readonly MarshalString assemblyPath;
    public readonly LanguageVersion csVersion;
    public readonly DisplayLanguage language;
    public readonly MarshalList<MarshalString> referenceDirs;
}