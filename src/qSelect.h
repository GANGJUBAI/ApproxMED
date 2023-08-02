// qSelect.h
// Code Source: https://www.techiedelight.com/quickselect-algorithm/
// Modify:Fenfang Li
// Time:2023/3

// Swap
#define _SWAP(T) SWAP$_##T##_$
#define SWAP(T) _SWAP(T)

#define _SWAP_IMPL(T)          \
    void SWAP(T)(T * x, T * y) \
    {                          \
        T temp = *x;           \
        *x = *y;               \
        *y = temp;             \
    }
#define SWAP_IMPL(T) _SWAP_IMPL(T)
SWAP_IMPL(int);
SWAP_IMPL(float);
SWAP_IMPL(double);

// Partition using Lomuto partition scheme
#define _partition(T) partition$_##T##_$
#define partition(T) _partition(T)

#define _partition_IMPL(T)                                                      \
    int partition(T)(T a[], int left, int right, int pIndex)                    \
    {                                                                           \
        /* pick `pIndex` as a pivot from the array*/                            \
        int pivot = a[pIndex];                                                  \
                                                                                \
        /*Move pivot to end*/                                                   \
        SWAP(T)                                                                 \
        (&a[pIndex], &a[right]);                                                \
                                                                                \
        /* elements less than the pivot will be pushed to the left of `pIndex`  \
           elements more than the pivot will be pushed to the right of `pIndex  \
        equal elements can go either way*/                                      \
        pIndex = left;                                                          \
                                                                                \
        /* each time we find an element less than or equal to the pivot, `pInd  \
           is incremented, and that element would be placed before the pivot.*/ \
        for (int i = left; i < right; i++)                                      \
        {                                                                       \
            if (a[i] <= pivot)                                                  \
            {                                                                   \
                SWAP(T)                                                         \
                (&a[i], &a[pIndex]);                                            \
                pIndex++;                                                       \
            }                                                                   \
        }                                                                       \
                                                                                \
        /*move pivot to its final place*/                                       \
        SWAP(T)                                                                 \
        (&a[pIndex], &a[right]);                                                \
                                                                                \
        /*return `pIndex` (index of the pivot element)*/                        \
        return pIndex;                                                          \
    }
#define partition_IMPL(T) _partition_IMPL(T)
partition_IMPL(int);
partition_IMPL(float);
partition_IMPL(double);

/* Returns the k'th smallest element in the list within `left…right`
 (i.e., left <= k <= right). The search space within the array is
 changing for each round – but the list is still the same size.
 Thus, `k` does not need to be updated with each round.*/
#define _quickselect(T) quickselect$_##T##_$
#define quickselect(T) _quickselect(T)

#define _quickselect_IMPL(T)                                                \
    T quickselect(T)(T nums[], int left, int right, int k)                  \
    {                                                                       \
        while (1)                                                           \
        {                                                                   \
            /*If the array contains only one element, return that element*/ \
            if (left == right)                                              \
            {                                                               \
                return nums[left];                                          \
            }                                                               \
                                                                            \
            /* select `pIndex` between left and right*/                     \
            int pIndex = left + rand() % (right - left + 1);                \
            /*int pIndex = (left +right)/2;*/                               \
            pIndex = partition(T)(nums, left, right, pIndex);               \
                                                                            \
            /*The pivot is in its final sorted position*/                   \
            if (k == pIndex)                                                \
            {                                                               \
                return nums[k];                                             \
            }                                                               \
                                                                            \
            /*if `k` is less than the pivot index*/                         \
            else if (k < pIndex)                                            \
            {                                                               \
                /*return quickselect(T)(nums, left, pIndex - 1, k);*/       \
                right = pIndex - 1;                                         \
            }                                                               \
                                                                            \
            /*if `k` is more than the pivot index*/                         \
            else                                                            \
            {                                                               \
                /*return quickselect(T)(nums, pIndex + 1, right, k);*/      \
                left = pIndex + 1;                                          \
            }                                                               \
        }                                                                   \
    }
#define quickselect_IMPL(T) _quickselect_IMPL(T)
quickselect_IMPL(int);
quickselect_IMPL(float);
quickselect_IMPL(double);
