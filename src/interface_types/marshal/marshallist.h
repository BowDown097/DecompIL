#pragma once
#include <QList>

template<typename T>
class MarshalList
{
public:
    template<typename U> requires std::is_constructible_v<T, const U&>
    MarshalList(const QList<U>& qList)
        : m_data(static_cast<T*>(aligned_alloc(alignof(T), sizeof(T) * qList.size()))),
          m_capacity(qList.capacity()),
          m_size(qList.size())
    {
        std::uninitialized_copy(qList.begin(), qList.end(), m_data);
    }

    MarshalList(const MarshalList& other)
        : m_data(static_cast<T*>(aligned_alloc(alignof(T), sizeof(T) * other.m_size))),
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
            out.append(std::move(m_data[i]));

        return out;
    }
private:
    T* m_data{};
    int m_capacity{};
    int m_size{};
};
