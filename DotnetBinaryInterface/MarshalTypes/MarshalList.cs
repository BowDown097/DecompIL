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
        _data = (T*)Marshal.AllocHGlobal(capacity * sizeof(T));
    }

    public T this[int index]
    {
        readonly get
        {
            if ((uint)index >= (uint)_size)
                throw new IndexOutOfRangeException();
            return _data[index];
        }
        set
        {
            if ((uint)index >= (uint)_size)
                throw new IndexOutOfRangeException();
            _data[index] = value;
        }
    }

    public readonly ref T At(int index) => ref _data[index];
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

        int reserve = capacity * sizeof(T);
        if (_capacity > 0)
            _data = (T*)Marshal.ReAllocHGlobal((nint)_data, reserve);
        else
            _data = (T*)Marshal.AllocHGlobal(reserve);

        _capacity = capacity;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    private readonly int GetNewCapacity() => _capacity > 0 ? 2 * _capacity : DefaultCapacity;
}