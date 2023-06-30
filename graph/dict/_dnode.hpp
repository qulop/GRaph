#ifndef __graph_dnode_hpp
#define __graph_dnode_hpp

namespace Graph
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

}

#endif 