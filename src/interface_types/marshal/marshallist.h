#pragma once
#include <QList>

template<typename T>
class MarshalList
{
public:
    ~MarshalList()
    {
        std::destroy_n(m_data, m_size);
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
    T* m_data;
    int m_capacity;
    int m_size;
};
