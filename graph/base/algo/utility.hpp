#ifndef __graph_algo_utility_hpp
#define __graph_algo_utility_hpp

#include <utility>

namespace Graph
{

    template<typename _Tx>
    void swap(_Tx& lhs, _Tx& rhs)
    {
        _Tx tmp = lhs;
        lhs = rhs;
        rhs = tmp;
    }
    
    
    template<typename _Tx>
    void swap(_Tx&& lhs, _Tx&& rhs)
    {
        _Tx tmp = std::move(lhs);
        lhs = std::move(rhs);
        rhs = std::move(tmp);
    }

}

#endif