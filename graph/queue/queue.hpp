#ifndef __gqueue_hpp
#define __gqueue_hpp

#include "../glist/list.hpp"


namespace Graph
{

template<typename _Tx, typename _Cont=List<_Tx>>
class Queue
{
private:

    _Cont container;

public:

    using container_type = _Cont;
    using value_type = _Tx;
    using reference = _Tx&;

    Queue(void) = default;

    Queue(const _Cont& container) :
        container(container) {}

    void push_back(_Tx value)
    { 
        container.push_back(value); 
    }

    value_type pop(void)
    { 
        _Tx value = container.front();
        container.pop_front();
        return value; 
    }

    reference front(void) const
    { 
        return container.front(); 
    }

    reference back(void) const
    { 
        return container.back(); 
    }

    constexpr size_t size(void) const
    { 
        return container.size(); 
    }

    constexpr Queue& operator=(const _Cont other)
    { 
        container = other; 
        return *this; 
    }

    constexpr Queue& operator=(const Queue& other)
    { 
        container = other.container; 
        return *this; 
    }

    ~Queue() = default;
};

}

#endif