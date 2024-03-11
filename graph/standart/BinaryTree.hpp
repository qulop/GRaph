#ifndef __graph_tree_dict_hpp
#define __graph_tree_dict_hpp

#include <initializer_list>
#include <utility>
#include <memory>
#include <cassert>


namespace Graph::Standart
{
    enum _DNColor    // Dictionary node color
    {
        _RED, _BLACK
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
    
        _DColor color = _BLACK;
        bool is_edge = false;

        _DNode(void) = default;

        _DNode(_Key key, _Value value) :
            key(key), value(value) {}

        void set(_Key key, _Value value)
        {
            this->key = key;
            this->value = value;
        }

        ~_DNode()
        { left = nullptr; right = nullptr; }
    };


    template<typename _Tree>
    class _TreeConstIterator
    {
    public:

        using _NodePtr = typename _Tree::_NodePtr;
        using ValueType = const typename _Tree::ValueType::second_type;
        using KeyType = const typename _Tree::KeyType;
        using Pointer = _NodePtr;
        using _MyIter = _TreeConstIterator<_Tree>;

    private:

        _NodePtr __next(void) noexcept
        {
            _NodePtr t_node =  node;

            if (t_node->right != nullptr)
            {
                t_node = t_node->right;
                while (t_node->left != nullptr)
                    t_node = t_node->left;
            } 
            else    // then rising up
            {
                _NodePtr parent = t_node->parent;
                while (parent != nullptr && t_node == parent->right)
                {
                    t_node = parent;
                    parent = t_node->parent;
                }
                t_node = parent;
            }

            return t_node;
        }

    public:

        _NodePtr node = nullptr;

        _TreeConstIterator(void) = delete;
    
        _TreeConstIterator(_NodePtr node) noexcept :
            node(node)
        {}

        _TreeConstIterator(_MyIter& other) noexcept :
            node(other.node)
        {}

        _TreeConstIterator(_MyIter&& other) noexcept :
            node(other.node)
        { other.node = nullptr; }

        _MyIter& next(void) noexcept
        { return ++*this; }

        bool has_next(void) const noexcept
        { return (__next() != nullptr); }

        KeyType& key(void) const noexcept
        { return node->key; }

        _MyIter& advance(size_t n) noexcept
        {
            for (size_t i = 0; has_next(); next())
                if (i == n - 1)
                    break;
            return *this;
        }

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

        ValueType& operator*(void) const noexcept
        { return node->value; }

        // Pointer operator->(void) const noexcept
        // { return std::pointer_traits<_NodePtr>::pointer_to(**this); }

        _MyIter& operator++(void) noexcept
        {
            node = __next();

            return *this;
        }

        _MyIter operator++(int) noexcept
        {
            _MyIter tmp = *this;
            ++*this;

            return tmp;
        }
    };


    template<typename _Key, typename _Value, typename _Alloc=std::allocator<_DNode<_Key, _Value>>>
    class Tree
    {
    // Red-Black tree implementation
    public:
        using KeyType = _Key;
        using ValueType = std::pair<KeyType, _Value>;
        using _Node = _DNode<_Key, _Value>;
        using _NodePtr = _Node*;

        using Iterator = _TreeConstIterator<Tree<_Key, _Value>>;    // REPLACE
        using ConstIterator = _TreeConstIterator<Tree<_Key, _Value>>;

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
        Tree(void) noexcept = default;

        Tree(const Tree& other) noexcept :
            root(root) {}

        Tree(Tree&& other) noexcept :
            root(other.root)
        { 
            other.root = nullptr; 
            other.dsize = 0;    
        }

        Tree(std::initializer_list<std::pair<_Key, _Value>> il) noexcept
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
        { return Iterator{ min() }; }

        Iterator end(void) const noexcept
        { return Iterator{ nullptr }; }

        ConstIterator cbegin(void) const noexcept
        { return static_cast<ConstIterator>(begin()); }

        ConstIterator cend(void) const noexcept
        { return static_cast<ConstIterator>(end()); }

        Tree& operator=(Tree&& other) noexcept
        {
            *this = std::move(other);   // memory leak
            return *this;
        }

        ~Tree() noexcept
        { clear(); }
    };

}

#endif

/*
* This library was dedicated to my lovely three suns: Amy, Clara and Victoria.
* From Yan with :heart:
*/