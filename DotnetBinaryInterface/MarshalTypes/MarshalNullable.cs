namespace DotnetBinaryInterface.MarshalTypes;

[StructLayout(LayoutKind.Sequential)]
public readonly unsafe struct MarshalNullable<T> where T : unmanaged
{
    private readonly bool _hasValue;
    private readonly T* _value;

    public MarshalNullable() {}

    public MarshalNullable(T? value)
    {
        if (value is not null)
        {
            _hasValue = true;
            _value = Utils.StructureToPtr(value.Value);
        }
    }

    public T? Value() => _hasValue ? *_value : null;

    public static implicit operator MarshalNullable<T>([DisallowNull] T value) => new(value);
}