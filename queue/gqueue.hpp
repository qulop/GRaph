#ifndef __gqueue_hpp
#define __gqueue_hpp

#include "../list/glist.hpp"


namespace gistd
{

template<typename Tx, typename Conatiner=List<Tx>>
class Queue
{
private:

    Conatiner container;

public:

    using container_type = Conatiner;
    using value_type = Tx;

    Queue(void) = default;

    Queue(const Conatiner& cont)
    { container = cont; }

    void push_back(Tx value)
    { container.push_back(value); }

    Tx pop(void)
    { 
        Tx value = container.front();
        container.pop_front();
        return value; 
    }

    Tx& front(void) const
    { return container.front(); }

    Tx& back(void) const
    { return container.back(); }

    constexpr size_t size(void) const
    { return container.size(); }

    constexpr Queue& operator=(const Conatiner cont)
    { container = cont; return *this; }

    constexpr Queue& operator=(const Queue& other)
    { container = other.container; return *this; }

    ~Queue(void) = default;
};

}

#endif