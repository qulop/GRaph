#ifndef __graph_dict_hpp
#define __graph_dict_hpp

#include <iterator>
#include <initializer_list>
#include <utility>
#include <memory>
#include <cassert>
#include "_dnode.hpp"

namespace Graph 
{

template<typename _Tree>
class _TreeBasicIterator
{
public:

    using _NodePtr = typename _Tree::_NodePtr;
    using ValueType = typename _Tree::ValueType;
    using Pointer = _NodePtr;
    using _MyIter = _TreeBasicIterator<_Tree>;


    _NodePtr node = nullptr;

    _TreeBasicIterator(void) = delete;
    
    _TreeBasicIterator(_NodePtr node) noexcept :
        node(node)
    {}

    _TreeBasicIterator(_MyIter& other) noexcept :
        node(other.node)
    {}

    _TreeBasicIterator(_MyIter&& other) noexcept :
        node(other.node)
    { other.node = nullptr; }

    _MyIter& next(void)
    { return ++*this; }

    bool has_next(void)
    { return node == nullptr; }

    _MyIter& operator=(const _MyIter& other) noexcept
    {
        node = other.node;
        return *this;
    }

    _MyIter& operator=(_MyIter&& other) noexcept
    {
        *this = other;
        other.node = nullptr;
        return *this;
    }

    bool operator==(const _MyIter& other) const noexcept
    { return node == other.node; };

    bool operator!=(const _MyIter& other) const noexcept
    { return !(*this == other.node); }

    ValueType operator*(void) const noexcept
    { return ValueType{ node->key, node->value }; }

    // Pointer operator->(void) const noexcept
    // { return std::pointer_traits<_NodePtr>::pointer_to(**this); }

    _MyIter& operator++(void) noexcept
    {
        if (node->left == nullptr)
        {
            node = node->right;
            return *this;
        }

        _NodePtr rmost = node->left;
        while (rmost->right != nullptr && rmost->right != node)
            rmost = rmost->right;

        if (rmost->right == nullptr)
        {
            rmost->right = node;
            node = node->left;
        } 
        else
        {
            rmost->right = nullptr;
            node = node->right;
            return *this;
        }

        return ++*this;
    }

    _MyIter operator++(int) noexcept
    {
        _MyIter tmp = node;
        ++*this;

        return tmp;
    }
};


template<typename _Key, typename _Value, typename _Alloc=std::allocator<_DNode<_Key, _Value>>>
class Dict
{
// Red-Black tree implementation
public:
    using KeyType = _Key;
    using ValueType = std::pair<KeyType, _Value>;
    using _Node = _DNode<_Key, _Value>;
    using _NodePtr = _Node*;

    using Iterator = _TreeBasicIterator<Dict<_Key, _Value>>;
    using ConstIterator = const _TreeBasicIterator<Dict<_Key, _Value>>;

private:

    _NodePtr root = nullptr;    
    size_t dsize = 0;


    _NodePtr __init_new_node(_NodePtr prnt, const _Key& key, const _Value& value) noexcept
    {
        _NodePtr node = new _Node{ key, value };
        node->parent = prnt;

        ++dsize;

        return node;
    }

    // Erasing without any childs
    Iterator __simple_erase(Iterator iter) noexcept
    {
        _NodePtr* parent = (iter.node)->parent;
        if (parent->left == iter.node)
            parent->left = nullptr;
        else
            parent->right = nullptr;
        delete iter.node;
        
        return Iterator{ parent };
    }

    Iterator __erase_wth_one_child(Iterator iter) noexcept
    {
        _NodePtr parent = (iter.node)->parent;
        _NodePtr child = ((iter.node)->left) ? 
                        (iter.node)->left : (iter.node)->right;

        if (parent->left == iter.node)
            parent->left = child;
        else
            parent->right = child;
        child->parent = parent;

        delete iter.node;
        
        return Iterator{ child };
    }

    // Erasing with two childs
    Iterator __hard_erase(Iterator iter) noexcept
    {
        Iterator min_node = min(iter.node->right);
        (iter.node)->set(min_node.node->key, min_node.node->value);

        __simple_erase(min_node);

        return iter;
    }

public:
    Dict(void) noexcept = default;

    Dict(const Dict& other) noexcept :
        root(root) {}

    Dict(Dict&& other) noexcept :
        root(other.root)
    { 
        other.root = nullptr; 
        other.dsize = 0;    
    }

    Dict(std::initializer_list<std::pair<_Key, _Value>> il) noexcept
    {
        for(auto& i : il)
            insert(i.first, i.second);
    }

    void insert(const _Key& key, const _Value& value) noexcept
    {
        if (root == nullptr)
        {  
            root = __init_new_node(nullptr, key, value); 
            return; 
        }
        
        assert(root != nullptr && dsize > 0);

        _NodePtr node = root;
        while (true) // while node isn't leaf
        {
            if (key == node->key) return;

            if (key < node->key)
            {
                if (node->left == nullptr)
                {
                    node->left = __init_new_node(node, key, value);
                    break;
                }
                node = node->left;
                continue;
            } 

            else if (node->right == nullptr)
            {
                node->right = __init_new_node(node, key, value);
                break;
            }
            node = node->right;
        }
    }

    Iterator at(const _Key& key) const noexcept
    {
        _NodePtr node = root;
        while (true)
        {
            if (node == nullptr)
                return end();

            if (key < node->key)
                node = node->left;
            else if (key > node->key)
                node = node->right;
            else
                return Iterator{ node };
        }
    }

    Iterator erase(Iterator iter) noexcept
    {
        if ((iter.node)->left == nullptr && (iter.node)->right == nullptr)
            return __simple_erase(iter);

        else if ((iter.node)->left != nullptr && (iter.node)->right != nullptr)
            return __hard_erase(iter);

        return __erase_wth_one_child(iter);
    }

    size_t size(void) const noexcept
    { return dsize; }

    void clear(void) noexcept
    {
        Iterator child = begin();
        Iterator node = ++(begin());

        for (node; node != end(); ++node)
        {
            delete child.node;
            child = node;
        }    

        dsize = 0;
        root = nullptr;
    }

    Iterator min(void) const noexcept
    { 
        _NodePtr node = root;
        while (node->left != nullptr)
            node = node->left;

        return Iterator{ node };
    }

    Iterator max(void) const noexcept
    { 
        _NodePtr node = root;
        while (node->right != nullptr)
            node = node->right;

        return Iterator{ node };    
    }

    Iterator begin(void) const noexcept
    { return Iterator{ root }; }

    Iterator end(void) const noexcept
    { return Iterator{ nullptr }; }

    ConstIterator cbegin(void) const noexcept
    { return static_cast<ConstIterator>(begin()); }

    ConstIterator cend(void) const noexcept
    { return static_cast<ConstIterator>(end()); }

    Dict& operator=(Dict&& other) noexcept
    {
        *this = std::move(other);
        return *this;
    }

    ~Dict() noexcept
    { clear(); }
};

}

#endif

/*
* This library was dedicated to my lovely three suns: Amy, Clara and Victoria.
* From Yan with :heart:
*/