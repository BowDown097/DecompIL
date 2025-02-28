#pragma once
#include <QString>

class MarshalString
{
public:
    MarshalString(const QString& qString)
        : m_data(new QChar[qString.size() + 1]), m_size(qString.size())
    {
        std::copy_n(qString.constData(), m_size, m_data);
        m_data[m_size] = QChar(0);
    }

    MarshalString(const MarshalString& other)
        : m_data(new QChar[other.m_size + 1]), m_size(other.m_size)
    {
        std::copy_n(other.m_data, m_size + 1, m_data);
    }

    MarshalString(MarshalString&& other)
    {
        other.swap(*this);
    }

    ~MarshalString()
    {
        delete[] m_data;
    }

    MarshalString& operator=(const MarshalString& other)
    {
        if (&other != this)
        {
            MarshalString temp(other);
            temp.swap(*this);
        }

        return *this;
    }

    MarshalString& operator=(MarshalString&& other)
    {
        MarshalString temp(std::move(other));
        temp.swap(*this);
        return *this;
    }

    void swap(MarshalString& other)
    {
        std::swap(m_data, other.m_data);
        std::swap(m_size, other.m_size);
    }

    QChar* data() noexcept { return m_data; }
    const QChar* data() const noexcept { return m_data; }

    int size() const { return m_size; }
    bool empty() const { return !m_size; }

    operator QString() const
    {
        return QString(m_data, m_size);
    }
private:
    QChar* m_data{};
    int m_size{};
};
