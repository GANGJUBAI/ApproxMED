// CAS_Main_qSelect.h
// Author:Fenfang Li
// Time:2023/3

#include "CAS_qSelect.h"

////////////////////////////////////////////// Get qselect Exact Median 1D /////////////////////////////////////////////
#define _getQselect1D(T) getQselect1D$_##T##_$
#define getQselect1D(T) _getQselect1D(T)

#define _getQselect1D_IMPL(T)                                                                                   \
    void getQselect1D(T)(int rows, T *data, T *Level_X, int Level_X_size)                                       \
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
            median = quickselect(T)(Level_X, 0, Level_X_size - 1, m);                                           \
        }                                                                                                       \
        gettimeofday(&end, NULL);                                                                               \
        double endtime = (double)1000 * (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000.0); \
        /*printf("qselect finding time is %fms\n", endtime);*/                                                      \
    }
#define getQselect1D_IMPL(T) _getQselect1D_IMPL(T)

getQselect1D_IMPL(int);
getQselect1D_IMPL(float);
getQselect1D_IMPL(double);

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
        getQselect1D(T)(rows, data, Level_X, Level_X_size);           \
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

////////////////////////////////////////////// Get qselect Exact Median 2D //////////////////////////////////////////////
#define _getQselect2D(T) getQselect2D$_##T##_$
#define getQselect2D(T) _getQselect2D(T)

#define _getQselect2D_IMPL(T)                                                                                   \
    void getQselect2D(T)(int rows, int cols, int layers, T *data, T *Level_X, int Level_X_size)                 \
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
            median = quickselect(T)(Level_X, 0, Level_X_size - 1, m);                                           \
        }                                                                                                       \
        gettimeofday(&end, NULL);                                                                               \
        double endtime = (double)1000 * (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000.0); \
        /*printf("qselect finding time is %fms\n", endtime);*/                                                      \
    }
#define getQselect2D_IMPL(T) _getQselect2D_IMPL(T)

getQselect2D_IMPL(int);
getQselect2D_IMPL(float);
getQselect2D_IMPL(double);

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
        getQselect2D(T)(rows, cols, layers, data, Level_X, Level_X_size); \
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
        getQselect2D(T)(rows, cols, Layers, data, Level_X, Level_X_size);              \
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
        getQselect2D(T)(rows, cols, layers, data, Level_X, Level_X_size);  \
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

////////////////////////////////////////////// Get qselect Exact Median ///////////////////////////////////////////////
#define _getQselect3D(T) getQselect3D$_##T##_$
#define getQselect3D(T) _getQselect3D(T)

#define _getQselect3D_IMPL(T)                                                                                   \
    void getQselect3D(T)(int rows, int cols, int layers, T *data, T *Level_X, int Level_X_size)                 \
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
                median = quickselect(T)(Level_X, 0, Level_X_size - 1, m);                                       \
            }                                                                                                   \
        }                                                                                                       \
        gettimeofday(&end, NULL);                                                                               \
        double endtime = (double)1000 * (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000.0); \
        /*printf("qselect finding time is %fms\n", endtime);*/                                                      \
    }
#define getQselect3D_IMPL(T) _getQselect3D_IMPL(T)

getQselect3D_IMPL(int);
getQselect3D_IMPL(float);
getQselect3D_IMPL(double);

/////////////////////// Process stream3D in 2D dataset ////////////////////////////
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
        getQselect3D(T)(rows, cols, layers, data, Level_X, Level_X_size);  \
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
        getQselect3D(T)(rows, cols, Layers, data, Level_X, Level_X_size);             \
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
process_stream3D3_IMPL(double);

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
        getQselect3D(T)(rows, cols, layers, data, Level_X, Level_X_size); \
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
