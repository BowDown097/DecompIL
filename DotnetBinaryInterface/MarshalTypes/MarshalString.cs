namespace DotnetBinaryInterface.MarshalTypes;

[StructLayout(LayoutKind.Sequential)]
public readonly unsafe struct MarshalString
{
    private readonly char* _data;
    private readonly int _length;

    public MarshalString(string? managedString)
    {
        if (managedString is not null)
        {
            _length = managedString.Length;

            int nb = (_length + 1) * 2;
            _data = (char*)Marshal.AllocHGlobal(nb);
            managedString.CopyTo(new Span<char>(_data, _length));
            _data[_length] = '\0';
        }
    }

    public MarshalString(StringHandle stringHandle, MetadataReader metadataReader, string? defaultString = null)
        : this(!stringHandle.IsNil ? metadataReader.GetString(stringHandle) : defaultString) {}

    public char this[int index]
    {
        readonly get
        {
            if ((uint)index >= (uint)_length)
                throw new IndexOutOfRangeException();
            return _data[index];
        }
        set
        {
            if ((uint)index >= (uint)_length)
                throw new IndexOutOfRangeException();
            _data[index] = value;
        }
    }

    public readonly int Length() => _length;

    public readonly bool StartsWith(string? managedString)
    {
        if (managedString is null || _length == 0)
            return false;

        ReadOnlySpan<char> managedStringSpan = managedString.AsSpan();
        ReadOnlySpan<char> dataSpan = new(_data, managedString.Length);
        return managedStringSpan.SequenceEqual(dataSpan);
    }

    public readonly override string? ToString() => _length > 0 ? new string(_data, 0, _length) : null;
}