#ifndef __glist_hpp
#define __glist_hpp

#include <iterator>
#include <initializer_list>
#include <utility>


// template<typename BasicListIteratorType>
// concept BidirectionalBasicListIteratorTraits = 
//     requires(BasicListIteratorType a)
//     {
//         { ++a } -> std::same_as<BasicListIteratorType&>;
//         { --a } -> std::same_as<BasicListIteratorType&>;
//     };


namespace gistd
{

template<typename Tx>
struct Node
{
    Tx data;
    Node* previous = this;
    Node* next = this;

    constexpr Node(void) = default;

    void set(Node* next, Node* previous)
    { this->next = next; this->previous = previous; }

    ~Node(void) = default;
};


template<typename Tx>
class BasicListIterator : public std::iterator<std::bidirectional_iterator_tag, Tx>
{
public:
    using Node = Node<Tx>;

    Node* node = nullptr;

    constexpr BasicListIterator(void) = default;

    explicit constexpr BasicListIterator(Node* node) :
        node(node) {}

    constexpr BasicListIterator(const BasicListIterator& other) :
        node(other.node) {}

    constexpr bool operator==(const BasicListIterator& other) const 
    { return node == other.node; }

    constexpr bool operator!=(const BasicListIterator& other) const
    { return !(*this == other); }

    Tx& operator*(void) const
    { return node->data; }

    BasicListIterator& operator++(void)
    { node = node->next; return *this; }

    BasicListIterator& operator--(void)
    { node = node->previous; return *this; }

    ~BasicListIterator(void)
    { node = nullptr; }
};


template<typename Tx>
class List
{
private:
    using Node = Node<Tx>;

    Node* edge; // point to itself as default
    size_t lsize = 0;

    void separate_node(Node* node)
    {
        node->previous->next = node->next;  // previous_node->next = node->next
        node->next->previous = node->previous;  // next_node->previous = node->previous
    }

public:
    using value_type = Tx;

    using Iterator = BasicListIterator<Tx>;
    using ConstIterator = const BasicListIterator<Tx>;

    List(void) :
        edge(new Node()) {}

    List(std::initializer_list<Tx> il)
    {
        for (auto& i : il)
            push_back(i);
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

    void push_back(Tx data)
    {
        Node* new_node = new Node();
        Node* previous_node = edge->previous;

        new_node->set(edge, previous_node);
        new_node->data = data;

        previous_node->next = new_node;

        if (lsize == 0)
            edge->set(new_node, new_node);
        edge->previous = new_node; 

        lsize += 1;
    }

    void push_front(Tx data)
    {
        Node* new_node = new Node();
        Node* start = edge->next;
        
        new_node->set(start, edge);
        new_node->data = data;
    
        start->previous = new_node;
        if (lsize == 0)
            edge->set(new_node, new_node);
        edge->next = new_node;

        lsize += 1;
    }

    size_t size(void) const
    { return lsize; }

    Tx pop_back(void)
    {
        Node* last_node = edge->previous;
        auto data = last_node->data;

        separate_node(last_node);

        // last_node->previous->next = edge; // the same as penulimate_node->next = edge
        // edge->previous = last_node->previous;

        delete last_node;

        lsize -= 1;

        return data;
    }

    Tx pop_front(void)
    {
        Node* first_node = edge->next;
        auto data = first_node->data;

        separate_node(first_node);

        // first_node->next->previous = edge;    // the same as second_node->previous = edge
        // edge->next = first_node->next;

        delete first_node;

        lsize -= 1;

        return data;
    }

    Iterator shift(Iterator target, Iterator place)
    {
        Node* node = target.node;
        separate_node(node);

        node->next = place.node->next;
        node->previous = place.node;

        place.node->next->previous = node;
        place.node->next = node;

        return target;
    }

    Iterator erase(ConstIterator pos)
    {
        Node* node = pos.node;
        separate_node(node);

        Iterator it{ node->next };

        delete node;

        lsize -= 1;

        return it;
    }

    void remove(const Tx& elem)
    {
        for (ConstIterator it = cbegin(); it != cend(); ++it)
            if ((*it) == elem)
            {
                erase(it);
                break;
            }
    }

    Tx& front(void) const
    { return edge->next->data; }

    Tx& back(void) const
    { return edge->previous->data; }

    Iterator begin(void) const
    { return Iterator{ edge->next }; }

    Iterator end(void) const
    { return Iterator{ edge }; }

    ConstIterator cbegin(void) const
    { return ConstIterator{ edge->next }; }

    ConstIterator cend(void) const
    { return ConstIterator{ edge }; }

    constexpr List& operator=(const List<Tx> other)
    { 
        edge = other.edge; 
        lsize = other.lsize; 
        return *this;    
    }

    constexpr void operator+=(const Tx& lhs)
    { push_back(lhs); }

    ~List(void)
    {
        if (edge == nullptr)    // if move-constructor/operator was called
            return;

        Iterator it = begin();
        for (it; it != end(); ++it)
            it = erase(it);
        erase(it);
    }
};

}

#endif