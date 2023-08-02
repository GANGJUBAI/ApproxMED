// Main_qsort.h
// Author: Fenfang Li
// Time:2023/3

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

////////////////////////////////////////////// Get qsort Exact Median 1D /////////////////////////////////////////////
#define _getQsort1D(T) getQsort1D$_##T##_$
#define getQsort1D(T) _getQsort1D(T)

#define _getQsort1D_IMPL(T)                                                                                     \
    void getQsort1D(T)(int rows, T *data, T *Level_X, int Level_X_size)                                         \
    {                                                                                                           \
        struct timeval start, end;                                                                              \
        double median = 0;                                                                                      \
        const size_t m = Level_X_size / 2 - 1;                                                                  \
                                                                                                                \
        gettimeofday(&start, NULL);                                                                             \
        for (int i = 0; i < rows; i++)                                                                          \
        {                                                                                                       \
            for (int j = 0; j < Level_X_size; j++)                                                              \
            {                                                                                                   \
                Level_X[j] = data[i * Level_X_size + j];                                                        \
            }                                                                                                   \
            qsort(Level_X, Level_X_size, sizeof(T), cmpfunc(T));                                                \
            median = data[m];                                                                                   \
        }                                                                                                       \
        gettimeofday(&end, NULL);                                                                               \
        double endtime = (double)1000 * (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000.0); \
        printf("qsort finding time is %fms\n", endtime);                                                        \
    }
#define getQsort1D_IMPL(T) _getQsort1D_IMPL(T)

getQsort1D_IMPL(int);
getQsort1D_IMPL(float);
getQsort1D_IMPL(double);

////////////////////////////////  Process stream 1D  //////////////////////////////
#define _process_stream1D(T) process_stream1D$_##T##_$
#define process_stream1D(T) _process_stream1D(T)

#define _process_stream1D_IMPL(T)                                     \
    int process_stream1D(T)(int rows, int cols, FILE *ori_fp)         \
    {                                                                 \
        int dataLength = rows * cols;                                 \
        int Level_X_size = cols;                                      \
                                                                      \
        T *data = (T *)malloc(dataLength * sizeof(T));                \
        size_t result = fread(data, Level_X_size, sizeof(T), ori_fp); \
        fclose(ori_fp);                                               \
        ori_fp = NULL;                                                \
        for (int i = 1; i < rows; i++)                                \
        {                                                             \
            for (int j = 0; j < Level_X_size; j++)                    \
            {                                                         \
                data[i * Level_X_size + j] = data[j];                 \
            }                                                         \
        }                                                             \
        T *Level_X = (T *)malloc(Level_X_size * sizeof(T));           \
        getQsort1D(T)(rows, data, Level_X, Level_X_size);             \
                                                                      \
        free(data);                                                   \
        data = NULL;                                                  \
        free(Level_X);                                                \
        Level_X = NULL;                                               \
        return 0;                                                     \
    }
#define process_stream1D_IMPL(T) _process_stream1D_IMPL(T)

process_stream1D_IMPL(int);
process_stream1D_IMPL(float);
process_stream1D_IMPL(double);

////////////////////////////////////////////// Get qsort Exact Median 2D //////////////////////////////////////////////
#define _getQsort2D(T) getQsort2D$_##T##_$
#define getQsort2D(T) _getQsort2D(T)

#define _getQsort2D_IMPL(T)                                                                                     \
    void getQsort2D(T)(int rows, int cols, int layers, T *data, T *Level_X, int Level_X_size)                   \
    {                                                                                                           \
        struct timeval start, end;                                                                              \
        double median = 0;                                                                                      \
        const size_t m = Level_X_size / 2 - 1;                                                                  \
                                                                                                                \
        gettimeofday(&start, NULL);                                                                             \
        for (int k = 0; k < layers; k++)                                                                        \
        {                                                                                                       \
            for (int i = 0; i < rows; i++)                                                                      \
            {                                                                                                   \
                for (int j = 0; j < cols; j++)                                                                  \
                {                                                                                               \
                    Level_X[i * cols + j] = data[k * rows * cols + i * cols + j];                               \
                }                                                                                               \
            }                                                                                                   \
            qsort(Level_X, Level_X_size, sizeof(T), cmpfunc(T));                                                \
            median = Level_X[m];                                                                                \
        }                                                                                                       \
        gettimeofday(&end, NULL);                                                                               \
        double endtime = (double)1000 * (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000.0); \
        printf("qsort finding time is %fms\n", endtime);                                                        \
    }
#define getQsort2D_IMPL(T) _getQsort2D_IMPL(T)

getQsort2D_IMPL(int);
getQsort2D_IMPL(float);
getQsort2D_IMPL(double);

////////////////////////////////  Process stream 2D  //////////////////////////////
#define _process_stream2D(T) process_stream2D$_##T##_$
#define process_stream2D(T) _process_stream2D(T)

#define _process_stream2D_IMPL(T)                                         \
    int process_stream2D(T)(int rows, int cols, int layers, FILE *ori_fp) \
    {                                                                     \
        int dataLength = rows * cols * layers;                            \
        int Level_X_size = rows * cols;                                   \
                                                                          \
        T *data = (T *)malloc(dataLength * sizeof(T));                    \
        size_t result = fread(data, Level_X_size, sizeof(T), ori_fp);     \
        fclose(ori_fp);                                                   \
        ori_fp = NULL;                                                    \
        for (int i = 1; i < layers; i++)                                  \
        {                                                                 \
            for (int k = 0; k < Level_X_size; k++)                        \
            {                                                             \
                data[i * Level_X_size + k] = data[k];                     \
            }                                                             \
        }                                                                 \
        T *Level_X = (T *)malloc(Level_X_size * sizeof(T));               \
                                                                          \
        getQsort2D(T)(rows, cols, layers, data, Level_X, Level_X_size);   \
                                                                          \
        free(data);                                                       \
        data = NULL;                                                      \
        free(Level_X);                                                    \
        Level_X = NULL;                                                   \
        return 0;                                                         \
    }
#define process_stream2D_IMPL(T) _process_stream2D_IMPL(T)

process_stream2D_IMPL(int);
process_stream2D_IMPL(float);
process_stream2D_IMPL(double);

/////////////////////  Process stream 2D in short 3D dataset  ///////////////////
#define _process_stream2Ds3(T) process_stream2Ds3$_##T##_$
#define process_stream2Ds3(T) _process_stream2Ds3(T)

#define _process_stream2Ds3_IMPL(T)                                                    \
    int process_stream2Ds3(T)(int rows, int cols, int layers, int loops, FILE *ori_fp) \
    {                                                                                  \
        int dataSize = rows * cols * layers;                                           \
        int dataLength = dataSize * loops;                                             \
        int row_size = dataSize;                                                       \
        int Level_X_size = rows * cols;                                                \
                                                                                       \
        T *data = (T *)malloc(dataLength * sizeof(T));                                 \
        size_t result = fread(data, dataSize, sizeof(T), ori_fp);                      \
        fclose(ori_fp);                                                                \
        ori_fp = NULL;                                                                 \
        for (int i = 1; i < loops; i++)                                                \
        {                                                                              \
            for (int k = 0; k < row_size; k++)                                         \
            {                                                                          \
                data[i * row_size + k] = data[k];                                      \
            }                                                                          \
        }                                                                              \
        T *Level_X = (T *)malloc(Level_X_size * sizeof(T));                            \
        int Layers = layers * loops;                                                   \
                                                                                       \
        getQsort2D(T)(rows, cols, Layers, data, Level_X, Level_X_size);                \
                                                                                       \
        free(data);                                                                    \
        data = NULL;                                                                   \
        free(Level_X);                                                                 \
        Level_X = NULL;                                                                \
        return 0;                                                                      \
    }
#define process_stream2Ds3_IMPL(T) _process_stream2Ds3_IMPL(T)

process_stream2Ds3_IMPL(int);
process_stream2Ds3_IMPL(float);
process_stream2Ds3_IMPL(double);

///////////////////////// Process stream 2D in 3D dataset ///////////////////////
#define _process_stream2D3(T) process_stream2D3$_##T##_$
#define process_stream2D3(T) _process_stream2D3(T)

#define _process_stream2D3_IMPL(T)                                         \
    int process_stream2D3(T)(int rows, int cols, int layers, FILE *ori_fp) \
    {                                                                      \
        int dataLength = rows * cols * layers;                             \
        int Level_X_size = rows * cols;                                    \
                                                                           \
        T *data = (T *)malloc(dataLength * sizeof(T));                     \
        size_t result = fread(data, dataLength, sizeof(T), ori_fp);        \
        fclose(ori_fp);                                                    \
        ori_fp = NULL;                                                     \
        T *Level_X = (T *)malloc(Level_X_size * sizeof(T));                \
                                                                           \
        getQsort2D(T)(rows, cols, layers, data, Level_X, Level_X_size);    \
                                                                           \
        free(data);                                                        \
        data = NULL;                                                       \
        free(Level_X);                                                     \
        Level_X = NULL;                                                    \
        return 0;                                                          \
    }
#define process_stream2D3_IMPL(T) _process_stream2D3_IMPL(T)

process_stream2D3_IMPL(int);
process_stream2D3_IMPL(float);
process_stream2D3_IMPL(double);

////////////////////////////////////////////// Get qsort Exact Median ///////////////////////////////////////////////
#define _getQsort3D(T) getQsort3D$_##T##_$
#define getQsort3D(T) _getQsort3D(T)

#define _getQsort3D_IMPL(T)                                                                                     \
    void getQsort3D(T)(int rows, int cols, int layers, T *data, T *Level_X, int Level_X_size)                   \
    {                                                                                                           \
        struct timeval start, end;                                                                              \
        double median = 0;                                                                                      \
        const size_t m = Level_X_size / 2 - 1;                                                                  \
                                                                                                                \
        gettimeofday(&start, NULL);                                                                             \
        for (int k = 0; k < layers; k++)                                                                        \
        {                                                                                                       \
            for (int i = 0; i < rows; i++)                                                                      \
            {                                                                                                   \
                for (int j = 0; j < cols; j++)                                                                  \
                {                                                                                               \
                    Level_X[j] = data[k * rows * cols + i * cols + j];                                          \
                }                                                                                               \
                qsort(Level_X, Level_X_size, sizeof(T), cmpfunc(T));                                            \
                median = Level_X[m];                                                                            \
                /*printf("Median[%d][%d]:%lg\n",k,i,median);*/                                                  \
            }                                                                                                   \
        }                                                                                                       \
        gettimeofday(&end, NULL);                                                                               \
        double endtime = (double)1000 * (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000.0); \
        printf("qsort finding time is %fms\n", endtime);                                                        \
    }
#define getQsort3D_IMPL(T) _getQsort3D_IMPL(T)

getQsort3D_IMPL(int);
getQsort3D_IMPL(float);
getQsort3D_IMPL(double);

////////////////////////  Process stream3D in 2D dataset ////////////////////////
#define _process_stream3D2(T) process_stream3D2$_##T##_$
#define process_stream3D2(T) _process_stream3D2(T)

#define _process_stream3D2_IMPL(T)                                         \
    int process_stream3D2(T)(int rows, int cols, int layers, FILE *ori_fp) \
    {                                                                      \
        int dataLength = rows * cols * layers;                             \
        int Level_X_size = cols;                                           \
        int row_size = rows * cols;                                        \
                                                                           \
        T *data = (T *)malloc(dataLength * sizeof(T));                     \
        size_t result = fread(data, dataLength, sizeof(T), ori_fp);        \
        fclose(ori_fp);                                                    \
        ori_fp = NULL;                                                     \
        for (int i = 1; i < layers; i++)                                   \
        {                                                                  \
            for (int k = 0; k < row_size; k++)                             \
            {                                                              \
                data[i * row_size + k] = data[k];                          \
            }                                                              \
        }                                                                  \
        T *Level_X = (T *)malloc(Level_X_size * sizeof(T));                \
                                                                           \
        getQsort3D(T)(rows, cols, layers, data, Level_X, Level_X_size);    \
                                                                           \
        free(data);                                                        \
        data = NULL;                                                       \
        free(Level_X);                                                     \
        Level_X = NULL;                                                    \
        return 0;                                                          \
    }
#define process_stream3D2_IMPL(T) _process_stream3D2_IMPL(T)

process_stream3D2_IMPL(int);
process_stream3D2_IMPL(float);
process_stream3D2_IMPL(double);

///////////////////////  Process stream3D in short 3D dataset //////////////////
#define _process_stream3D3(T) process_stream3D3$_##T##_$
#define process_stream3D3(T) _process_stream3D3(T)

#define _process_stream3D3_IMPL(T)                                                    \
    int process_stream3D3(T)(int rows, int cols, int layers, int loops, FILE *ori_fp) \
    {                                                                                 \
        int dataSize = rows * cols * layers;                                          \
        int dataLength = dataSize * loops;                                            \
        int Level_X_size = cols;                                                      \
        int row_size = dataSize;                                                      \
                                                                                      \
        T *data = (T *)calloc(dataLength, sizeof(T));                                 \
        size_t result = fread(data, dataSize, sizeof(T), ori_fp);                     \
        fclose(ori_fp);                                                               \
        ori_fp = NULL;                                                                \
        for (int i = 1; i < loops; i++)                                               \
        {                                                                             \
            for (int k = 0; k < row_size; k++)                                        \
            {                                                                         \
                data[i * row_size + k] = data[k];                                     \
            }                                                                         \
        }                                                                             \
        T *Level_X = (T *)calloc(Level_X_size, sizeof(T));                            \
        int Layers = layers * loops;                                                  \
                                                                                      \
        getQsort3D(T)(rows, cols, Layers, data, Level_X, Level_X_size);               \
                                                                                      \
        free(data);                                                                   \
        data = NULL;                                                                  \
        free(Level_X);                                                                \
        Level_X = NULL;                                                               \
        return 0;                                                                     \
    }
#define process_stream3D3_IMPL(T) _process_stream3D3_IMPL(T)

process_stream3D3_IMPL(int);
process_stream3D3_IMPL(float);

////////////////////////////////  Process stream3D ///////////////////////////////
#define _process_stream3D(T) process_stream3D$_##T##_$
#define process_stream3D(T) _process_stream3D(T)

#define _process_stream3D_IMPL(T)                                         \
    int process_stream3D(T)(int rows, int cols, int layers, FILE *ori_fp) \
    {                                                                     \
        int dataLength = rows * cols * layers;                            \
        int Level_X_size = cols;                                          \
                                                                          \
        T *data = (T *)malloc(dataLength * sizeof(T));                    \
        size_t result = fread(data, dataLength, sizeof(T), ori_fp);       \
        fclose(ori_fp);                                                   \
        ori_fp = NULL;                                                    \
        T *Level_X = (T *)malloc(Level_X_size * sizeof(T));               \
                                                                          \
        getQsort3D(T)(rows, cols, layers, data, Level_X, Level_X_size);   \
                                                                          \
        free(data);                                                       \
        data = NULL;                                                      \
        free(Level_X);                                                    \
        Level_X = NULL;                                                   \
        return 0;                                                         \
    }
#define process_stream3D_IMPL(T) _process_stream3D_IMPL(T)

process_stream3D_IMPL(int);
process_stream3D_IMPL(float);
process_stream3D_IMPL(double);
