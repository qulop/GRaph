#ifndef RUBY_VECTOR_HPP
#define RUBY_VECTOR_HPP

#include <iterator>


namespace Ruby::Standart
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


#endif // RUBY_VECTOR_HPP