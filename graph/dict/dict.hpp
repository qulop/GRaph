/*
TODO:
    1) Assign operator for Dict
    2) Make DictIter bidirectional (IN PROCESS)
*/
#ifndef __gdict_hpp
#define __gdict_hpp

#include <iterator>
#include <initializer_list>
#include <utility>
#include <memory>

namespace Graph 
{

#define _CalledIncrementTag 0
#define _CalledDecrementTag 1
#define _IsSubtreeTop(node) ((node->left != nullptr && node->right != nullptr)) 
// In this case, Subtree Top - it is node, 
// that stricly have two childs nodes

enum _DNColor    // Dictionary node color
{
    RED, BLACK
};


template<typename _Key, typename _Value>
struct _DNode
{
    using _DColor = _DNColor;

    _Key key;
    _Value value;

    _DNode* left = nullptr;
    _DNode* right = nullptr;
    _DNode* parent = nullptr;
    
    _DColor color = BLACK;

    _DNode(void) = default;

    _DNode(_Key key, _Value value) :
        key(key), value(value) {}

    ~_DNode(void)
    { left = nullptr; right = nullptr; }
};


template<typename _Key, typename _Value>
class _BasicDictionaryIterator : public std::iterator<std::bidirectional_iterator_tag, _Key>
{
public:
    using _DNode = _DNode<_Key, _Value>;

private:

    // dn - mean __get_deepest_node() function
    _DNode* __get_condition_way_for_dn(_DNode* node, bool tag)
    { return (tag == _CalledIncrementTag) ? node->left : node->right; }

    _DNode* __get_condition_way(_DNode* node, bool tag)
    { return (tag == _CalledIncrementTag) ? node->right : node->left; }

    _DNode* __get_deepest_node(_DNode* node, bool tag)
    {
        while (true)
        {
           if (__get_condition_way_for_dn(node, tag) == nullptr)
                return node;
            node = node->left;
        }
    }

    // returns next node in depend from calling operator(++/--)
    // <bool tag> - operaion(0 - increment, 1 - decrement)
    _DNode* __next(_DNode* node, bool tag)
    {
        if (__get_condition_way(node, tag) != nullptr)
            return __get_deepest_node(__get_condition_way(node, tag), tag);

        while (true)
        {
            _DNode* parent = node->parent;
            if (parent == nullptr)
                return nullptr;

            if (!_IsSubtreeTop(parent) || node != __get_condition_way(parent, tag))
                return parent;
            node = parent;
        }
    }

public:

    _DNode* node;

    _BasicDictionaryIterator(void) = default;

    explicit _BasicDictionaryIterator(_DNode* node) :
        node(node) 
    {}

    _BasicDictionaryIterator(const _BasicDictionaryIterator& other) :
        node(other.node) 
    {}

    _BasicDictionaryIterator(_BasicDictionaryIterator&& other)
    {
        node = other.node;
        other.node = nullptr;
    }

    _BasicDictionaryIterator& operator++(void)
    {
        node = __next(node, _CalledIncrementTag);         
        return *this;
    }

    _BasicDictionaryIterator operator++(int)
    {
        auto tmp = *this;
        ++*this; 
        return tmp; 
    }

    _BasicDictionaryIterator& operator--(void)
    {
        node = __next(node, _CalledDecrementTag);
        return *this;
    }

    _BasicDictionaryIterator operator--(int)
    {
        auto tmp = *this;
        --*this; 
        return tmp; 
    }

    _BasicDictionaryIterator& operator=(const _BasicDictionaryIterator& other) 
    {   
        node = other.node; 
        return *this; 
    }

    _BasicDictionaryIterator& operator=(_BasicDictionaryIterator&& other)
    {
        node = std::move(other);
        return *this;
    }

    constexpr bool operator==(const _BasicDictionaryIterator& other) const
    { return (node == other.node); } 

    constexpr bool operator==(const _DNode* other) const
    { return node == other; }

    constexpr bool operator!=(const _BasicDictionaryIterator& other) const
    { return !(*this == other.node); }

    _Value& operator*(void) const
    { return node->value; }
};


template<typename _Key, typename _Value>
class _DictionaryReverseIterator : public _BasicDictionaryIterator<_Key, _Value> 
{
    
};


template<typename _Key, typename _Value>
class Dict
{
// Basic tree implementation
public:
    using key_type = _Key;
    using value_type = _Value;

    using _DNode = _DNode<_Key, _Value>;

    using Iterator = _BasicDictionaryIterator<_Key, _Value>;
    using ConstIterator = const _BasicDictionaryIterator<_Key, _Value>;
    using ReverseIterator = _DictionaryReverseIterator<_Key, _Value>;
    using ConstReverseIterator = const _DictionaryReverseIterator<_Key, _Value>; 

private:
    _DNode* root = nullptr;
    size_t dsize = 0;

    void __create_edge_node(void)
    {
        root->parent = new _DNode();
        root->parent->left = root;
    }

    Iterator __dmin(_DNode* node)  const
    {
        while (node->left != nullptr)
            node = node->left;

        return Iterator{ node };
    }

    Iterator __dmax(_DNode* node) const
    {
        while (node->right != nullptr)
            node = node->right;

        return Iterator{ node };
    }

    // Erasing without any childs
    Iterator __simple_erase(Iterator iter)
    {
        _DNode* parent = (iter.node)->parent;
        if (parent->left == iter.node)
            parent->left = nullptr;
        else
            parent->right = nullptr;
        delete iter.node;
        
        return Iterator{ parent };
    }

    Iterator __erase_wth_one_child(Iterator iter)
    {
        _DNode* parent = (iter.node)->parent;
        _DNode* child = ((iter.node)->left) ? 
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
    Iterator __hard_erase(Iterator iter)
    {
        Iterator min_node = __dmin(iter.node->right);
        (iter.node)->key = min_node.node->key;
        (iter.node)->value = min_node.node->value;

        __simple_erase(min_node);

        return iter;
    }

    void __delete_branch(Iterator vertex)
    {
        auto child = __dmin(vertex);
    }

public:
    Dict(void) = default;

    Dict(const Dict& other) :
        root(root) {}

    Dict(Dict&& other):
        root(other.root)
    { 
        other.root = nullptr; 
        other.size = 0;    
    }

    Dict(std::initializer_list<std::pair<_Key, _Value>> il)
    {
        for(auto& i : il)
            insert(i.first, i.second);
    }

    virtual void insert(const _Key& key, const _Value& value)
    {
        if (root == nullptr)
        { 
            root = new _DNode(key, value);
            __create_edge_node();
            return;
        }

        _DNode* node = root;
        while (true) // while node isn't leaf
        {
            if (key == node->key)
            { return; }

            if (key < node->key)
            {
                if (node->left == nullptr)
                {
                    node->left = new _DNode(key, value);
                    node->left->parent = node;
                    break;
                }
                node = node->left;
                continue;
            } 

            else if (node->right == nullptr)
            {
                node->right = new _DNode(key, value);
                node->right->parent = node;

                break;
            }
            node = node->right;
        }

        ++dsize;
    }

    Iterator at(const _Key& key) const
    {
        _DNode* node = root;
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

    virtual Iterator erase(Iterator iter)
    {
        if ((iter.node)->left == nullptr && (iter.node)->right == nullptr)
            return __simple_erase(iter);

        else if ((iter.node)->left != nullptr && (iter.node)->right != nullptr)
            return __hard_erase(iter);

        return __erase_wth_one_child(iter);
    }

    size_t size(void) const
    { return dsize; }

    void clear(void)
    {
        auto child = begin();
        auto node = ++(begin());

        for (node; node != end(); ++node)
        {
            delete child.node;
            child = node;
        }    

        dsize = 0;
        root = nullptr;
    }

    Iterator min(void) const
    { return __dmin(root); }

    Iterator max(void) const
    { return __dmax(root); }

    Iterator begin(void) const
    { return __dmin(root); }

    Iterator end(void) const
    { return Iterator{ root->parent }; }

    ConstIterator cbegin(void) const
    { return begin(); }

    ConstIterator cend(void) const
    { return end(); }

    ReverseIterator rbegin(void) const
    { return __dmax(root); }

    ReverseIterator rend(void) const
    { return ReverseIterator{ nullptr }; }

    ConstReverseIterator crbegin(void) const
    { return rbegin(); }

    ConstReverseIterator crend(void) const
    { return rend(); }

    Dict& operator=(const Dict& other)
    {

    }

    Dict& operator=(Dict&& other)
    {
        *this = std::move(other);
        return *this;
    }

    ~Dict()
    {
        clear();
    }
};

}

#endif