#ifndef RUBY_STACK_HPP
#define RUBY_STACK_HPP

#include <deque>


namespace Ruby::Standart
{
    template<typename _Tx, typename _Base=std::deque<_Tx>>
    class Stack
    {
    private:

        _Base base;
        size_t ssize;

    public:

        using ValueType = _Tx
        using BaseType = _Base;
        using Reference = ValueType&;
        using ConstReference = const Reference;

        Stack(void) noexcept = default;

        Stack(const _Base& init_val) noexcept :
            base(init_val) {}

        void push(const value_type& value) noexcept
        { 
            ssize += 1;
            return base.push_back(value); 
        }

        bool empty(void) const noexcept
        { return base.empty(); }

        template<typename... _Args>
        void emplace(_Args&&... args)
        { base.emplace(args...); }

        ValueType pop(void) noexcept
        { 
            ssize -= 1;
            return base.pop_back(); 
        }

        size_t size(void) const noexcept
        { return ssize; }

        ~Stack() = default;
    };

}

#endif // RUBY_STACK_HPP