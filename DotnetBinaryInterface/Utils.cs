namespace DotnetBinaryInterface;

internal static class Utils
{
    internal static unsafe T* StructureToPtr<T>([DisallowNull] T structure) where T : unmanaged
    {
        T* _ptr = (T*)NativeMemory.Alloc((nuint)sizeof(T));
        Unsafe.Write(_ptr, structure);
        return _ptr;
    }
}