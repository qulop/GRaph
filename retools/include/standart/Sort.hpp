#ifndef RUBY_SORT_HPP
#define RUBY_SORT_HPP


namespace Ruby::Standart
{
    template<typename Tx>
    struct Quicksort
    {
    public:
        static void Sort(Tx& arr)
        { QsRecurse(arr, 0, arr.size() - 1); }

    private:
        template<typename Tx>
        static int QsPartition(Tx& arr, int l, int h)
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


        template<typename Tx>
        static void QsRecurse(Tx& arr, int l, int h)
        {
            if (h - l < 0)
                return;
            int p = _qs_partition(arr, l, h);
            QsRecurse(arr, l, p - 1);
            QsRecurse(arr, p + 1, h);
        }
    };


    template<typename Tx>
    void bubbleSort(Tx& arr)
    {
        for (auto i = 0; i < arr.size(); i++)
            for (auto j = 0; j < arr.size() - 1; j++)
                if (arr[i] < arr[j])
                    swap(arr[i], arr[j]);
    }
    
    
    template<typename Tx>
    void selectionSort(Tx& arr)
    {
        for (auto i = 0; i < arr.size() - 1; i++)
            for (auto j = i + 1; j < arr.size(); j++)
                if (arr[j] < arr[i])
                    swap(arr[i], arr[j]);
    }
    
    
    template<typename Tx>
    void insertionSort(Tx& arr)
    {
        for (auto i = 1; i < arr.size(); i++)
            for (auto j = i; (j > 0 && arr[j] < arr[j - 1]); j--)
                swap(arr[j], arr[j - 1]);
    }
}

#endif // RUBY_SORT_HPP