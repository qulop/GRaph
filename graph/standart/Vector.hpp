#ifndef __gvec_hpp
#define __gvec_hpp

#include <iterator>


namespace Graph::Standart
{
    template<typename Tx>
    class BasicVectorIterator : std::iterator<std::forward_iterator_tag, Tx>
    {

    }; 


    template<typename Tx>
    class Vector
    {
    public:
        using Iterator = BasicVectorIterator<Tx>;
        using ConstIterator = const BasicVectorIterator<Tx>;

    };

}


#endif