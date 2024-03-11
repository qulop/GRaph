#ifndef __graph_list_hpp
#define __graph_list_hpp

// #define NDEBUG

#include <memory>
#include <cassert>
#include <utility>
#include <iterator>
#include <initializer_list>


namespace Graph::Standart
{
    template<typename _Tx>
    struct _LNode
    {
        _Tx data;
        _LNode* previous = this;
        _LNode* next = this;
    
        _LNode(void) = default;
    
        _LNode(const _Tx data) :
            data(data)
        {}
    
        void set(_LNode* next, _LNode* previous)
        { 
            this->next = next; 
            this->previous = previous; 
        }
    
        ~_LNode()
        {
            previous = nullptr;
            next = nullptr;
        };
    };
    
    
    template<typename _Tx>
    class _BasicListIterator : public std::iterator<std::bidirectional_iterator_tag, _Tx>
    {
    public:
        using _LNode = _LNode<_Tx>;
    
        _LNode* node = nullptr;
    
        _BasicListIterator(void) = default;
    
        explicit _BasicListIterator(_LNode* node) :
            node(node) {}
    
        _BasicListIterator(const _BasicListIterator& other) :
            node(other.node) {}
    
        _BasicListIterator(_BasicListIterator&& other) :
            node(other.node)
        { other.node = nullptr; }
    
        constexpr bool operator==(const _BasicListIterator& other) const 
        { return node == other.node; }
    
        constexpr bool operator!=(const _BasicListIterator& other) const
        { return !(*this == other); }
    
        _Tx& operator*(void) const
        { return node->data; }
    
        _Tx* operator->(void) const
        { return node; }
    
        _BasicListIterator& operator++(void)
        { 
            node = node->next; 
            return *this; 
        }
    
        _BasicListIterator operator++(int)
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }
    
        _BasicListIterator& operator--(void)
        { 
            node = node->previous; 
            return *this; 
        }
    
        _BasicListIterator operator--(int)
        {
            auto tmp = *this;
            --*this;
            return tmp;
        }
    
        ~_BasicListIterator()
        { node = nullptr; }
    };
    
    
    // Allocators type must support standard STL interface
    template<typename _Tx, typename _Allocator=std::allocator<_Tx>>
    class List
    {
    public:
        using value_type = _Tx;
        using reference = _Tx&;
        using pointer = _Tx*;
    
        using Iterator = _BasicListIterator<_Tx>;
        using ConstIterator = const _BasicListIterator<_Tx>;
        // using ReverseIterator = _ListReverseIterator<_Tx>;
        // using ConstReverseIterator = const _ListReverseIterator<_Tx>;
    
    private:
        using _LNode = _LNode<_Tx>;
        using _Alloc = _Allocator;
        using _NodeAlloc = std::allocator_traits<_LNode>;
    
        _Alloc allocator;
        _LNode* edge; // points to itself as default
        size_t lsize = 0;
    
        void __separate_node(_LNode* node)
        {
            node->previous->next = node->next;  // previous_node->next = node->next
            node->next->previous = node->previous;  // next_node->previous = node->previous
        }
    
        [[nodiscard]] 
        Iterator __copy_until_reaches_value(size_t value, const List& other)
        {
            Iterator _our_it = begin();
            Iterator _alien_it = other.begin();
            size_t _i = 0;
    
            for (_i; _i < value; _our_it++, _alien_it++, _i++)
                _our_it->data = _alien_it->data;
    
            return (lsize > other.lsize) ? ++_our_it : ++_alien_it;        
        }
    
        void __erase_extra_nodes(Iterator iter)
        {
            for (iter; iter != end(); iter++)
                erase(iter);
        }
    
        void __add_missing_nodes(Iterator iter, ConstIterator end)
        {
            for (iter; iter != end; iter++)
                push_back(*iter);
        }
    
    public:
    
        List(void) :
            edge(new _LNode{}) {}
    
        List(std::initializer_list<_Tx> il)
        {
            for (auto& _i : il)
                push_back(_i);
        }
    
        List(const List& other) :
            edge(other.edge), lsize(other.lsize) {}
    
        List(List&& other) :
            edge(other.edge), 
            lsize(other.lsize)
        { 
            other.edge = nullptr;
            other.lsize = 0;    
        }
    
        void push_back(_Tx data)
        {
            _LNode* _new_node = allocator.allocate(1);
            _LNode* _previous_node = edge->previous;
    
            _new_node->set(edge, _previous_node);
            _new_node->data = data;
    
            _previous_node->next = _new_node;
    
            if (lsize == 0)
                edge->set(_new_node, _new_node);
            edge->previous = _new_node; 
    
            lsize += 1;
        }
    
        void push_front(_Tx data)
        {
            _LNode* _new_node = allocator.allocate(1);
            _LNode* _start = edge->next;
            
            _new_node->set(_start, edge);
            _new_node->data = data;
        
            _start->previous = _new_node;
            if (lsize == 0)
                edge->set(_new_node, _new_node);
            edge->next = _new_node;
    
            lsize += 1;
        }
    
        size_t size(void) const
        { return lsize; }
    
        _Tx pop_back(void)
        {
            _LNode* _last_node = edge->previous;
            auto _data = _last_node->data;
    
            __separate_node(_last_node);
    
            allocator.deallocate(_last_node, 1);
    
            lsize -= 1;
    
            return _data;
        }
    
        _Tx pop_front(void)
        {
            _LNode* _first_node = edge->next;
            auto data = _first_node->data;
    
            __separate_node(_first_node);
    
            allocator.deallocate(_first_node, 1);
    
            lsize -= 1;
    
            return data;
        }
    
        Iterator shift(Iterator target, Iterator place)
        {
            _LNode* _node = target.node;
            __separate_node(_node);
    
            _node->next = place.node->next;
            _node->previous = place.node;
    
            place.node->next->previous = _node;
            place.node->next = _node;
    
            return target;
        }
    
        template<typename... Args>
        void emplace(Args&&... args)
        {
        
        }
    
        template<typename... Args>
        void emplace_front(Args&&... args)
        {
            
        }
    
        Iterator erase(ConstIterator pos)
        {
            _LNode* _node = pos.node;
            __separate_node(_node);
    
            Iterator _it{ _node->next };
    
            allocator.deallocate(_node, 1);
    
            lsize -= 1;
    
            return _it;
        }
    
        void remove(const _Tx& elem)
        {
            for (ConstIterator _it = cbegin(); _it != cend(); ++_it)
            {
                if ((*_it) == elem)
                {
                    erase(_it);
                    break;
                }
            }
        }
    
        _Tx& front(void) const
        { return edge->next->data; }
    
        _Tx& back(void) const
        { return edge->previous->data; }
    
        Iterator begin(void) const
        { return Iterator{ edge->next }; }
    
        Iterator end(void) const
        { return Iterator{ edge }; }
    
        ConstIterator cbegin(void) const
        { return begin(); }
    
        ConstIterator cend(void) const
        { return end(); }
    
        ReverseIterator rbegin(void) const
        { return ReverseIterator{ edge->previous }; }
    
        ReverseIterator rend(void) const
        { return ReverseIterator{ edge }; }
    
        ConstReverseIterator crbegin(void) const
        { return rbegin(); }
    
        ConstReverseIterator crend(void) const
        { return rend(); }
    
        List& operator=(const List& other)
        { 
            edge->data = other.edge->data;
            auto _iter = __copy_until_reaches_value((lsize < other.size) ? lsize : other.lsize);
    
            if (lsize > other.lsize)
                __erase_extra_nodes(_iter);
            else if (lsize < other.lsize)
                __add_missing_nodes(_iter, other.end());
            
            lsize = other.lsize; 
            return *this;    
        }
    
        List& operator=(List&& other)
        {
            *this = std::move(other);
            return *this;
        }
    
        List& operator+=(const _Tx& lhs)
        { 
            push_back(lhs); 
            return *this;
        }
    
        List& operator+=(const List& other)
        {
            __add_missing_nodes(other.begin(), other.end());
            lsize += other.lsize;
    
            return *this;
        }
    
        ~List()
        {
            if (edge != nullptr)    // if move-constructor/operator wasn't called
            {
                Iterator it = begin();
                for (it; it != end(); ++it)
                    it = erase(it);
                erase(it);
            }
        }
    };

}

#endif