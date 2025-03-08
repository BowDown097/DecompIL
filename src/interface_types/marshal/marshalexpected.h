#pragma once
#include <stdexcept>

template<typename T, typename E>
class MarshalExpected
{
    template<typename U, typename G>
    friend class MarshalExpected;

    template<typename U, typename G>
    static constexpr bool cons_from_expected = std::disjunction_v<
        std::is_constructible<std::remove_cvref_t<U>, std::remove_cvref_t<MarshalExpected<U, G>>>,
        std::is_constructible<std::remove_cvref_t<G>, std::remove_cvref_t<MarshalExpected<U, G>>>>;

    template<typename U, typename G>
    static constexpr bool explicit_conv = std::disjunction_v<
        std::negation<std::is_convertible<U, T>>, std::negation<std::is_convertible<G, E>>>;
public:
    MarshalExpected()
        noexcept(std::is_nothrow_default_constructible_v<T>)
        requires std::is_default_constructible_v<T>
        : m_hasValue(true), m_ptr(new T)
    { }

    MarshalExpected(const MarshalExpected& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_constructible<T>,
                                    std::is_nothrow_copy_constructible<E>>)
        : m_hasValue(other.m_hasValue)
    {
        if (m_hasValue)
            m_ptr = new T(*static_cast<T*>(other.m_ptr));
        else
            m_ptr = new E(*static_cast<E*>(other.m_ptr));
    }

    MarshalExpected(MarshalExpected&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_constructible<T>,
                                    std::is_nothrow_move_constructible<E>>)
        : m_ptr(other.m_ptr), m_hasValue(other.m_hasValue)
    {
        other.m_ptr = nullptr;
    }

    template<typename U, typename G>
        requires std::is_constructible_v<T, const U&> &&
                 std::is_constructible_v<E, const G&> &&
                 (!cons_from_expected<U, G>)
    explicit(explicit_conv<const U&, const G&>)
        MarshalExpected(const MarshalExpected<U, G>& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T, const U&>,
                                    std::is_nothrow_constructible<E, const G&>>)
        : m_hasValue(other.m_hasValue)
    {
        if (m_hasValue)
            m_ptr = new T(*static_cast<U*>(other.m_ptr));
        else
            m_ptr = new E(*static_cast<G*>(other.m_ptr));
    }

    template<typename U, typename G>
        requires std::is_constructible_v<T, U> &&
                 std::is_constructible_v<E, G> &&
                 (!cons_from_expected<U, G>)
    explicit(explicit_conv<U, G>)
        MarshalExpected(MarshalExpected<U, G>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T, U>,
                                    std::is_nothrow_constructible<E, G>>)
        : m_hasValue(other.m_hasValue)
    {
        if (m_hasValue)
            m_ptr = new T(std::move(*static_cast<U*>(other.m_ptr)));
        else
            m_ptr = new E(std::move(*static_cast<G*>(other.m_ptr)));
        other.m_ptr = nullptr;
    }

    template<typename U = T>
        requires (!std::is_same_v<std::remove_cvref_t<U>, MarshalExpected>) && std::is_constructible_v<T, U>
    explicit(!std::is_convertible_v<U, T>)
        MarshalExpected(U&& value)
        noexcept(std::is_nothrow_constructible_v<T, U>)
        : m_ptr(new T(std::forward<U>(value))), m_hasValue(true)
    { }

    template<typename G = E>
        requires std::is_constructible_v<E, G>
    explicit(!std::is_convertible_v<G, E>)
        MarshalExpected(G&& error)
        noexcept(std::is_nothrow_constructible_v<E, G>)
        : m_ptr(new E(std::forward<G>(error))), m_hasValue(false)
    { }

    ~MarshalExpected()
    {
        if (m_ptr)
        {
            if (m_hasValue)
                delete static_cast<T*>(m_ptr);
            else
                delete static_cast<E*>(m_ptr);
        }
    }

    const T* operator->() const noexcept { return static_cast<T*>(m_ptr); }
    T* operator->() noexcept { return static_cast<T*>(m_ptr); }

    template<class Self>
    auto&& operator*(this Self&& self) noexcept
    { return *static_cast<T*>(std::forward<Self>(self).m_ptr); }

    explicit operator bool() const noexcept { return m_hasValue; }
    bool has_value() const noexcept { return m_hasValue; }

    template<class Self>
    auto&& value(this Self&& self)
    {
        if (self.has_value())
            return *static_cast<T*>(std::forward<Self>(self).m_ptr);
        throw std::runtime_error("Expected type has no value.");
    }

    template<class Self>
    auto&& error(this Self&& self) noexcept
    { return *static_cast<E*>(std::forward<Self>(self).m_ptr); }
private:
    void* m_ptr;
    bool m_hasValue;
};
