#include "TEST_iterator_methods.hpp"
#include <iostream>
#include <map>


int main(void)
{
    Graph::Dict<int, int> dict = {
        { 6, 6 },
        { 2, 2 },
        { 1, 1 },
        { 4, 4 },
        { 3, 3 },
        { 5, 5 },
        { 7, 7 },
        { 9, 9 },
        { 8, 8 }
    };

    Graph::Dict<int, int>::Iterator it = dict.begin();
    it.next();
    for (it; it.has_next(); it.next())
        std::cout << (*it).first << std::endl;
}