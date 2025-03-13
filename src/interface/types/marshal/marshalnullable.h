#pragma once
#include <stdexcept>

template<typename T>
class MarshalNullable
{
public:
    MarshalNullable() noexcept = default;

    MarshalNullable(const MarshalNullable& other)
        noexcept(std::is_nothrow_copy_constructible_v<T>)
        : m_hasValue(other.m_hasValue), m_value(new T(*other.m_value)) {}

    MarshalNullable(MarshalNullable&& other)
        noexcept(std::is_nothrow_move_constructible_v<T>)
        : m_hasValue(other.m_hasValue), m_value(other.m_value)
    {
        other.m_value = nullptr;
    }

    template<typename U>
        requires std::is_constructible_v<T, const U&>
    explicit(!std::is_convertible_v<const U&, T>)
        MarshalNullable(const MarshalNullable<U>& other)
        noexcept(std::is_nothrow_constructible_v<T, const U&>)
        : m_hasValue(other.m_hasValue), m_value(new T(*other.m_value)) {}

    template<typename U>
        requires std::is_constructible_v<T, U>
    explicit(!std::is_convertible_v<U, T>)
        MarshalNullable(MarshalNullable<U>&& other)
        noexcept(std::is_nothrow_constructible_v<T, U>)
        : m_hasValue(other.m_hasValue), m_value(new T(std::move(*other.m_value)))
    {
        other.m_value = nullptr;
    }

    template<typename U = std::remove_cv_t<T>>
        requires (!std::is_same_v<std::remove_cvref_t<U>, MarshalNullable<T>>) && std::is_constructible_v<T, U>
    explicit(!std::is_convertible_v<U, T>)
        MarshalNullable(U&& other)
        noexcept(std::is_nothrow_constructible_v<T, U>)
        : m_hasValue(true), m_value(new T(std::forward<U>(other))) {}

    ~MarshalNullable()
    {
        if (m_value)
            delete m_value;
    }

    const T* operator->() const noexcept { return m_value; }
    T* operator->() noexcept { return m_value; }

    template<class Self>
    auto&& operator*(this Self&& self) noexcept
    { return *std::forward<Self>(self).m_value; }

    explicit operator bool() const noexcept { return m_hasValue; }
    bool has_value() const noexcept { return m_hasValue; }

    template<class Self>
    auto&& value(this Self&& self)
    {
        if (self.has_value())
            return *std::forward<Self>(self).m_value;
        throw std::runtime_error("Nullable type has no value.");
    }
private:
    bool m_hasValue{};
    T* m_value{};
};
