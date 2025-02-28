namespace DotnetBinaryInterface.MarshalTypes;

[StructLayout(LayoutKind.Sequential)]
public readonly unsafe struct MarshalExpected<T, E>
    where T : unmanaged
    where E : unmanaged
{
    private readonly nint _ptr;
    private readonly bool _hasValue;

    public MarshalExpected(T v)
    {
        _hasValue = true;
        _ptr = (nint)Utils.StructureToPtr(v);
    }

    public MarshalExpected(E e)
    {
        _hasValue = false;
        _ptr = (nint)Utils.StructureToPtr(e);
    }

    public T Value()
    {
        if (!_hasValue)
            throw new InvalidOperationException("Expected type holds no value.");
        return *(T*)_ptr;
    }

    public E Error() => *(E*)_ptr;
    public bool HasValue() => _hasValue;

    public static implicit operator MarshalExpected<T, E>([DisallowNull] T value) => new(value);
    public static implicit operator MarshalExpected<T, E>([DisallowNull] E error) => new(error);
}