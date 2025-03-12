namespace DotnetBinaryInterface;

internal static class Utils
{
    internal static unsafe T* StructureToPtr<T>([DisallowNull] T structure, bool fDeleteOld = false)
        where T : unmanaged
    {
        T* _ptr = (T*)Marshal.AllocHGlobal(sizeof(T));
        Marshal.StructureToPtr(structure, (nint)_ptr, fDeleteOld);
        return _ptr;
    }
}