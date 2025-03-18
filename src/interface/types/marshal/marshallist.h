#pragma once
#include <QList>

template<typename T>
class MarshalList
{
public:
    MarshalList() = default;

    template<typename U> requires std::is_constructible_v<T, const U&>
    MarshalList(const QList<U>& qList)
        : m_data(allocateData(qList.size())),
          m_capacity(qList.capacity()),
          m_size(qList.size())
    {
        std::uninitialized_copy(qList.begin(), qList.end(), m_data);
    }

    MarshalList(const MarshalList& other)
        : m_data(allocateData(other.m_size)),
          m_capacity(other.m_capacity),
          m_size(other.m_size)
    {
        std::uninitialized_copy_n(other, other.m_size, m_data);
    }

    MarshalList(MarshalList&& other)
    {
        other.swap(*this);
    }

    ~MarshalList()
    {
        std::destroy_n(m_data, m_size);
        freeData();
    }

    MarshalList& operator=(const MarshalList& other)
    {
        if (&other != this)
        {
            MarshalList temp(other);
            temp.swap(*this);
        }

        return *this;
    }

    MarshalList& operator=(MarshalList&& other)
    {
        MarshalList temp(std::move(other));
        temp.swap(*this);
        return *this;
    }

    void swap(MarshalList& other)
    {
        std::swap(m_data, other.m_data);
        std::swap(m_capacity, other.m_capacity);
        std::swap(m_size, other.m_size);
    }

    template<typename U> requires std::is_constructible_v<U, const T&>
    operator QList<U>() const&
    {
        QList<U> out;
        out.reserve(m_size);

        for (int i = 0; i < m_size; ++i)
            out.append(m_data[i]);

        return out;
    }

    template<typename U> requires std::is_constructible_v<U, T>
    operator QList<U>() &&
    {
        QList<U> out;
        out.reserve(m_size);

        for (int i = 0; i < m_size; ++i)
            out.emplaceBack(std::move(m_data[i])); // emplaceBack is used to preserve move, which afaik append doesn't

        return out;
    }
private:
    T* m_data{};
    int m_capacity{};
    int m_size{};

    T* allocateData(qsizetype count)
    {
    #if defined(_MSC_VER) || defined(__MINGW32_MAJOR_VERSION)
        return static_cast<T*>(_aligned_malloc(sizeof(T) * count, alignof(T)));
    #else
        return static_cast<T*>(std::aligned_alloc(alignof(T), sizeof(T) * count));
    #endif
    }

    void freeData()
    {
    #if defined(_MSC_VER) || defined(__MINGW32_MAJOR_VERSION)
        _aligned_free(m_data);
    #else
        std::free(m_data);
    #endif
    }
};
