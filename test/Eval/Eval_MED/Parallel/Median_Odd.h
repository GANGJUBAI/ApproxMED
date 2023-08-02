#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define __type_is_int(expr) __builtin_types_compatible_p(typeof(expr), int)

#define _cmpfunc(T) cmpfunc$_##T##_$
#define cmpfunc(T) _cmpfunc(T)

#define _cmpfunc_IMPL(T)                         \
    int cmpfunc(T)(const void *a, const void *b) \
    {                                            \
        T res = *(T *)a - *(T *)b;               \
        if (__type_is_int(T))                    \
            return res;                          \
        else                                     \
            return (res > 0.0 ? 1 : -1);         \
    }
#define cmpfunc_IMPL(T) _cmpfunc_IMPL(T)
cmpfunc_IMPL(int);
cmpfunc_IMPL(float);
cmpfunc_IMPL(double);

/////////////////////////////////////////// Get Median_odd //////////////////////////////////
#define _Median_Odd(T) Median_Odd$_##T##_$
#define Median_Odd(T) _Median_Odd(T)

#define _Median_Odd_IMPL(T)                                                                      \
    T Median_Odd(T)(T * Array, int dataLength)                                                   \
    {                                                                                            \
                                                                                                 \
        int fillNum;                                                                             \
        int length;                                                                              \
        int dataSize;                                                                            \
        T median;                                                                                \
                                                                                                 \
        length = ceill(sqrt(dataLength));                                                        \
                                                                                                 \
        /*adjust length*/                                                                        \
        if (length % 2 == 0)                                                                     \
        {                                                                                        \
            length += 1;                                                                         \
        }                                                                                        \
        dataSize = length * length;                                                              \
                                                                                                 \
        /*compute number of fill block */                                                        \
        fillNum = dataSize - dataLength;                                                         \
                                                                                                 \
        T *tempArray = (T *)calloc(dataSize, sizeof(T));                                         \
        T **newArray = (T **)calloc(dataSize, sizeof(T));                                        \
                                                                                                 \
        for (int i = fillNum; i < dataSize; i++)                                                 \
        {                                                                                        \
            tempArray[i] = Array[i - fillNum];                                                   \
        }                                                                                        \
        for (int i = 0; i < length; i++)                                                         \
        {                                                                                        \
            newArray[i] = &tempArray[(int)length * i];                                           \
        }                                                                                        \
                                                                                                 \
        /*sort columns in ascending order*/                                                      \
        for (int j = 0; j < length; j++)                                                         \
        {                                                                                        \
            T t;                                                                                 \
            for (int m = length - 1; m > 0; m--)                                                 \
            {                                                                                    \
                for (int n = 0; n < m; n++)                                                      \
                {                                                                                \
                    if (newArray[n][j] > newArray[n + 1][j])                                     \
                    {                                                                            \
                        t = newArray[n][j];                                                      \
                        newArray[n][j] = newArray[n + 1][j];                                     \
                        newArray[n + 1][j] = t;                                                  \
                    }                                                                            \
                }                                                                                \
            }                                                                                    \
        }                                                                                        \
                                                                                                 \
        /*sort rows in ascending order*/                                                         \
        for (int j = 0; j < length; j++)                                                         \
        {                                                                                        \
            qsort(newArray[j], length, sizeof(T), cmpfunc(T));                                   \
        }                                                                                        \
                                                                                                 \
        /*diagonalsort*/                                                                         \
        for (int k = 0; k < length; k++)                                                         \
        {                                                                                        \
            for (int i = 0; i < length; i++)                                                     \
            {                                                                                    \
                for (int j = length - 1; j >= 0; j--)                                            \
                {                                                                                \
                    if (i + 1 < length && j - 1 > -1 && newArray[i][j] > newArray[i + 1][j - 1]) \
                    {                                                                            \
                        T t = newArray[i][j];                                                    \
                        newArray[i][j] = newArray[i + 1][j - 1];                                 \
                        newArray[i + 1][j - 1] = t;                                              \
                    }                                                                            \
                }                                                                                \
            }                                                                                    \
        }                                                                                        \
                                                                                                 \
        median = newArray[(length - 1) / 2][(length - 1) / 2];                                   \
                                                                                                 \
        free(tempArray);                                                                         \
        free(newArray);                                                                          \
                                                                                                 \
        return median;                                                                           \
    }
#define Median_Odd_IMPL(T) _Median_Odd_IMPL(T)

Median_Odd_IMPL(float);
Median_Odd_IMPL(double);
