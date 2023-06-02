#ifndef __gdict_hpp
#define __gdict_hpp

#include <iterator>
#include <initializer_list>
#include <utility>
#include <iostream>


namespace gistd
{

template<typename Key, typename Value>
struct DNode
{
    using DColor = char;

    Key key;
    Value value;

    DNode* left = nullptr;
    DNode* right = nullptr;
    DNode* parent = nullptr;
    DColor color = 'b';

    DNode(void) = default;

    DNode(Key key, Value value) :
        key(key), value(value) {}

    void ptr_set(DNode* left, DNode* right)
    { this->left = left, this->right = right; }

    ~DNode(void)
    { left = nullptr; right = nullptr; }
};


template<typename Key, typename Value>
class BasicDictionaryIterator : public std::iterator<std::bidirectional_iterator_tag, Key>
{
public:
    using DNode = DNode<Key, Value>;

private:

    bool __is_subtree_top(DNode)
    {
        
    }

    DNode* __get_deepest_node(DNode* node)
    {
        while (true)
            if (node->left == nullptr)
                return node;
    }


    DNode* __next(DNode* node)
    {
        if (node->left != nullptr)
            return __get_deepest_node(node->left);
        if (node->right != nullptr)
            return __get_deepest_node(node->right);

        return node->parent;
    }

public:

    DNode* node;

    BasicDictionaryIterator(void) = default;

    explicit BasicDictionaryIterator(DNode* node) :
        node(node) 
    {}

    BasicDictionaryIterator(const BasicDictionaryIterator& other) :
        node(other.node) 
    {}

    BasicDictionaryIterator& operator++(void)
    {
        while (true)
        {
            node = __next(node);

        }
            

        return *this;
    }

    BasicDictionaryIterator& operator--(void)
    {

        return *this;
    }

    BasicDictionaryIterator& operator=(const BasicDictionaryIterator& other)
    {   
        node = other.node; 
        return *this; 
    }

    constexpr bool operator==(const BasicDictionaryIterator& other)
    { return (node == other.node); }

    constexpr bool operator==(const DNode* other)
    { return node == other; }

    constexpr bool operator!=(const BasicDictionaryIterator& other)
    { return !(*this == other.node); }

    Value& operator*(void) const
    { return node->value; }
};


template<typename Key, typename Value>
class BasicDictionaryReverseIterator : public BasicDictionaryIterator<Key, Value>
{

};


template<typename Key, typename Value>
class Dictionary
{
public:
    using key_type = Key;
    using value_type = Value;

    using DNode = DNode<Key, Value>;

    using Iterator = BasicDictionaryIterator<Key, Value>;
    using ConstIterator = const BasicDictionaryIterator<Key, Value>;
    using ReverseIterator = BasicDictionaryReverseIterator<Key, Value>;
    using ConstReverseIterator = const BasicDictionaryReverseIterator<Key, Value>; 

private:
    DNode* root = nullptr;

    Iterator __dmin(Iterator from)  const
    {
        DNode* node = from.node;
        while (node->left != nullptr)
            node = node->left;

        return Iterator{ node };
    }

    Iterator __dmax(Iterator from) const
    {
        DNode* node = from.node;
        while (node->right != nullptr)
            node = node->right;

        return Iterator{ node };
    }


    Iterator __simple_erase(Iterator iter)
    {
        DNode* parent_n = (iter.node)->parent;
        if (parent_n->left == iter)
            parent_n->left = nullptr;
        else
            parent_n->right = nullptr;
        delete iter.node;
        
        return Iterator{ parent_n };
    }


    Iterator __erase_wth_one_child(Iterator iter)
    {
        DNode* parent = (iter.node)->parent;
        
        

    }

    Iterator __hard_erase(Iterator iter)
    {

    }

public:
    Dictionary(void) = default;

    Dictionary(const Dictionary& other) :
        root(root) {}

    Dictionary(std::initializer_list<std::pair<Key, Value>> il)
    {
        for(auto& i : il)
            insert(i.first, i.second);
    }

    void insert(Key key, Value value)
    {
        if (root == nullptr)
        { 
            root = new DNode(key, value);
            return;
        }

        DNode* node = root;
        while (true) // while node isn't leaf
        {
            if (key == node->key)
            { return; }

            if (key < node->key)
            {
                if (node->left == nullptr)
                {
                    node->left = new DNode(key, value);
                    node->left->parent = node;
                    break;
                }
                node = node->left;
                continue;
            } 

            else if (node->right == nullptr)
            {
                node->right = new DNode(key, value);
                node->right->parent = node;
                break;
            }
            node = node->right;
        }
    }

    Iterator find(Key key)
    {
        DNode* node = root;
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

    // Iterator erase(Iterator iter)
    // {
    //     DNode* parent_n = (iter.node)->parent;

    //     if ((iter.node)->left == nullptr && (iter.node)->right == nullptr)
    //         return __simple_erase(iter);

    //     else if ((iter.node)->left != nullptr || (iter.node)->right != nullptr)
    //         return __erase_wth_one_child(iter);
    // }

    Iterator min(void) const
    { return __dmin(Iterator{ root }); }

    Iterator max(void) const
    { return __dmax(Iterator{ root }); }

    Iterator begin(void) const
    { return __dmin(Iterator{ root }); }

    Iterator end(void) const
    { return __dmax(Iterator{ root }); }

    ConstIterator cbegin(void) const
    { ConstIterator{ min() }; }

    ConstIterator cend(void) const
    { ConstIterator{ max() }; }

    // Dictionary& operator=(const Dictionary& other)
    // {
    //     root = other.root;
    //     return *this;
    // }

    ~Dictionary(void) = default;    // FIX
    
};

}

#endif