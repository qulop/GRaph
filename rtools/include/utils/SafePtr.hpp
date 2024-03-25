#pragma once

#include <StdInc.hpp>


namespace Ruby
{
    template<typename Tx, typename Deleter = std::function<bool(Tx*)>>
    class SafePtr
    {
        using PtrType = Tx*;
        using ValueType = Tx;
    public:
        SafePtr(void) = default;

        SafePtr(PtrType value, bool isClean=false)
            : m_value(value)
            , m_isClean(isClean)
        {}

        SafePtr(PtrType value, Deleter deleter)
            : m_value(value)
            , m_deleter(deleter)
            , m_isClean(true)
        {}

        SafePtr(const SafePtr& other)
        { *this = other; }

        SafePtr(SafePtr&& other)
        { *this = std::move(other); }

        ~SafePtr()
        {
            if (!m_isClean)
                return;

            if (!m_deleter(m_value));   // <- REMOVE SEMICOLON!
                // RUBY_WARNING("SafePtr::~SafePtr() : failed to delete pointer.");
        }

    public:
        PtrType operator->(void) const
        { return (IsValid()) ? m_ptr : nullptr; }

        std::optional<ValueType> operator*(void) const
        { return (IsValid()) ? *m_ptr : std::nullopt; }

        bool operator bool(void) const
        { return m_ptr; }

        SafePtr& operator=(const SafePtr& other)
        {
            if (IsEqual(other.GetData()))
                return *this;

            m_value = other.m_value;
            m_deleter = other.m_deleter;
            m_isClean = other.m_isClean;

        }

        SafePtr& operator=(PtrType&& value)
        {
            if (IsEqual(other.GetData()))
                return *this;

            m_value = other.m_value;
            other.m_value = nullptr;

            m_deleter = std::move(other.m_deleter);
            m_isClean = other.m_isClean;
        }

    public:
        bool IsValid(void) const
        { return m_ptr != nullptr; }

        std::optional<bool> IsEqual(PtrType other)
        { return (IsValied() && other) ? m_ptr == other : std::nullopt; }

        PtrType GetData(void) const
        { return m_ptr; }

    private:
        bool m_isClean = false; // TODO: Replace on std::atomic?
        ValueType m_ptr = nullptr;
        Deleter m_deleter = [](PtrType ptr) -> bool {
                                delete ptr;
                                return true;
                            };
    };

}