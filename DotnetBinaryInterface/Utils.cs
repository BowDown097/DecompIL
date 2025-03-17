namespace DotnetBinaryInterface;

internal static class Utils
{
    [StructLayout(LayoutKind.Sequential)]
    private struct AlignOfHelper<T> where T : struct
    {
        public byte dummy;
        public T data;
    }

    internal static nuint AlignOf<T>() where T : struct
    {
        return (nuint)(Unsafe.SizeOf<AlignOfHelper<T>>() - Unsafe.SizeOf<T>());
    }

    internal static unsafe T* StructureToPtr<T>([DisallowNull] T structure) where T : unmanaged
    {
        T* _ptr = (T*)NativeMemory.Alloc((nuint)sizeof(T));
        Unsafe.Write(_ptr, structure);
        return _ptr;
    }
}