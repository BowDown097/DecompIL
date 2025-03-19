namespace DotnetBinaryInterface.MarshalTypes;

// freeing is done from the native side
[StructLayout(LayoutKind.Sequential)]
public unsafe struct MarshalList<T> where T : unmanaged
{
    private const int DefaultCapacity = 4;

    private T* _data;
    private int _capacity;
    private int _size;

    public MarshalList() {}

    public MarshalList(int capacity)
    {
        _capacity = capacity;
        _data = (T*)NativeMemory.AlignedAlloc(checked((nuint)capacity * (nuint)sizeof(T)), Utils.AlignOf<T>());
    }

    public readonly ref T this[int index] => ref _data[index];

    public readonly ref T At(int index)
    {
        if ((uint)index >= (uint)_size)
            throw new IndexOutOfRangeException();
        return ref _data[index];
    }

    public readonly int Capacity() => _capacity;
    public readonly int Size() => _size;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public void Add([DisallowNull] T item)
    {
        if ((uint)_capacity <= (uint)_size)
            Reserve(GetNewCapacity());

        _data[_size++] = item;
    }

    public void Reserve(int capacity)
    {
        capacity = Math.Min(capacity, Array.MaxLength);
        if (capacity <= _capacity)
            return;

        nuint reserve = checked((nuint)capacity * (nuint)sizeof(T));
        if (_capacity > 0)
            _data = (T*)NativeMemory.AlignedRealloc(_data, reserve, Utils.AlignOf<T>());
        else
            _data = (T*)NativeMemory.AlignedAlloc(reserve, Utils.AlignOf<T>());

        _capacity = capacity;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    private readonly int GetNewCapacity() => _capacity > 0 ? 2 * _capacity : DefaultCapacity;
}