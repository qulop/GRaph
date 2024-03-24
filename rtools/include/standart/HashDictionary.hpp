#ifndef RUBY_HASH_MAP_HPP
#define RUBY_HASH_MAP_HPP

#include <vector>
#include <functional>
#include <iostream>
#include <type_traits>


namespace Ruby::Standart
{

    template<typename _Key, typename _Value>
    class HashDictionary
    {
    public:
        using KeyType = _Key;
        using ValueType = _Value;

        HashDictionary(void)
        {
            m_vec.reserve(5);
        }

        HashDictionary(_Key key, _Value val);

        size_t insert(KeyType key, ValueType val)
        {
            if (std::is_integral_v<KeyType>)
                return key % m_vec.capacity();

            size_t raw_hash = std::hash<KeyType>{}(key);

            std::cout << raw_hash << " " << m_vec.capacity() << std::endl;

            return raw_hash % m_vec.capacity();
        }

    private:
        std::vector<ValueType> m_vec;
    };

}


#endif