#ifndef __graph_algo_sort_hpp
#define __graph__algo_sort_hpp

#include "utility.hpp"
#include <iostream>


namespace Graph
{
    template<typename _Tx>
    int _qs_partition(_Tx& arr, int l, int h)
    {
        int fh = l;
        int p = h;

        for (int i = l; i < h; i++)
        {
            if (arr[i] < arr[p])
            {
                swap(arr[i], arr[fh]);
                fh++;
            }
        }

        swap(arr[p], arr[fh]);
        return fh;
    }


    template<typename _Tx>
    void _qs_recurse(_Tx& arr, int l, int h)
    {
        if (h - l < 0)
            return;
        int p = _qs_partition(arr, l, h);
        _qs_recurse(arr, l, p - 1);    
        _qs_recurse(arr, p + 1, h);    
    }


    template<typename _Tx>
    void quicksort(_Tx& arr)
    { _qs_recurse(arr, 0, arr.size() - 1); }




    template<typename _Tx>
    void bubble_sort(_Tx& arr)
    {
        for (auto i = 0; i < arr.size(); i++)
            for (auto j = 0; j < arr.size() - 1; j++)
                if (arr[i] < arr[j])
                    swap(arr[i], arr[j]);
    }
    
    
    template<typename _Tx>
    void selection_sort(_Tx& arr)
    {
        for (auto i = 0; i < arr.size() - 1; i++)
            for (auto j = i + 1; j < arr.size(); j++)
                if (arr[j] < arr[i])
                    swap(arr[i], arr[j]);
    }
    
    
    template<typename _Tx>
    void insertion_sort(_Tx& arr)
    {
        for (auto i = 1; i < arr.size(); i++)
            for (auto j = i; (j > 0 && arr[j] < arr[j - 1]); j--)
                swap(arr[j], arr[j - 1]);
    }
}

#endif