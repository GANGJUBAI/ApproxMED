// Main_FSM_ori.h
// Author:Fenfang Li
// Time:2023/5

#include "ApproxMD_FG.h"

////////////////////////////////////////////// Get FSM Approximate Median 1D ////////////////////////////////////////////
#define _getAMD1D(T) getAMD1D$_##T##_$
#define getAMD1D(T) _getAMD1D(T)

#define _getAMD1D_IMPL(T)                                                                                       \
    void getAMD1D(T)(int rows, T *data, T *Level_X, int Level_X_size)                                           \
    {                                                                                                           \
        struct timeval start, end;                                                                              \
        double mean = 0, median = 0, sigma = 0;                                                                 \
        int findMedianStage = 0;                                                                                \
                                                                                                                \
        gettimeofday(&start, NULL);                                                                             \
        for (int i = 0; i < rows; i++)                                                                          \
        {                                                                                                       \
            for (int j = 0; j < Level_X_size; j++)                                                              \
            {                                                                                                   \
                Level_X[j] = data[i * Level_X_size + j];                                                        \
            }                                                                                                   \
            switch (findMedianStage)                                                                            \
            {                                                                                                   \
            case 0:                                                                                             \
                median = medianFindStage1(T)(Level_X, 47, Level_X_size, &mean, &sigma);                         \
                findMedianStage = 1;                                                                            \
                break;                                                                                          \
            case 1:                                                                                             \
                median = medianFindStage2(T)(Level_X, 47, Level_X_size, &mean, &sigma);                         \
                break;                                                                                          \
            }                                                                                                   \
        }                                                                                                       \
        gettimeofday(&end, NULL);                                                                               \
        double endtime = (double)1000 * (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000.0); \
        printf("Approximate_FSM_FG finding time is %fms\n", endtime);                                          \
    }
#define getAMD1D_IMPL(T) _getAMD1D_IMPL(T)

getAMD1D_IMPL(int);
getAMD1D_IMPL(float);
getAMD1D_IMPL(double);

//////////////////////////////  Process stream 1D  //////////////////////////////
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
        getAMD1D(T)(rows, data, Level_X, Level_X_size);               \
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

////////////////////////////////////////////// Get Approximate Median 2D ////////////////////////////////////////////////
#define _getAMD2D(T) getAMD2D$_##T##_$
#define getAMD2D(T) _getAMD2D(T)

#define _getAMD2D_IMPL(T)                                                                                       \
    void getAMD2D(T)(int rows, int cols, int layers, T *data, T *Level_X, int Level_X_size)                     \
    {                                                                                                           \
        struct timeval start, end;                                                                              \
        double mean = 0, median = 0, sigma = 0;                                                                 \
        int findMedianStage = 0;                                                                                \
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
            switch (findMedianStage)                                                                            \
            {                                                                                                   \
            case 0:                                                                                             \
                median = medianFindStage1(T)(Level_X, 47, Level_X_size, &mean, &sigma);                         \
                findMedianStage = 1;                                                                            \
                break;                                                                                          \
            case 1:                                                                                             \
                median = medianFindStage2(T)(Level_X, 47, Level_X_size, &mean, &sigma);                         \
                break;                                                                                          \
            }                                                                                                   \
        }                                                                                                       \
        gettimeofday(&end, NULL);                                                                               \
        double endtime = (double)1000 * (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000.0); \
        printf("Approximate_FSM_FG finding time is %fms\n", endtime);                                                      \
    }
#define getAMD2D_IMPL(T) _getAMD2D_IMPL(T)

getAMD2D_IMPL(int);
getAMD2D_IMPL(float);
getAMD2D_IMPL(double);

////////////////////////////////  Process stream 2D  ////////////////////////////
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
        getAMD2D(T)(rows, cols, layers, data, Level_X, Level_X_size);     \
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
        getAMD2D(T)(rows, cols, Layers, data, Level_X, Level_X_size);                  \
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
        getAMD2D(T)(rows, cols, layers, data, Level_X, Level_X_size);      \
                                                                           \
        /*printf("T is int :%ld\n",sizeof(T));*/                           \
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

////////////////////////////////////////////// Get FSM Approximate Median 3D ////////////////////////////////////////////
#define _getAMD3D(T) getAMD3D$_##T##_$
#define getAMD3D(T) _getAMD3D(T)

#define _getAMD3D_IMPL(T)                                                                                       \
    void getAMD3D(T)(int rows, int cols, int layers, T *data, T *Level_X, int Level_X_size)                     \
    {                                                                                                           \
        struct timeval start, end;                                                                              \
        double mean = 0, median = 0, sigma = 0;                                                                 \
        int findMedianStage = 0;                                                                                \
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
                switch (findMedianStage)                                                                        \
                {                                                                                               \
                case 0:                                                                                         \
                    median = medianFindStage1(T)(Level_X, 47, Level_X_size, &mean, &sigma);                     \
                    findMedianStage = 1;                                                                        \
                    break;                                                                                      \
                case 1:                                                                                         \
                    median = medianFindStage2(T)(Level_X, 47, Level_X_size, &mean, &sigma);                     \
                    break;                                                                                      \
                }                                                                                               \
            }                                                                                                   \
        }                                                                                                       \
        gettimeofday(&end, NULL);                                                                               \
        double endtime = (double)1000 * (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000.0); \
        printf("Approximate_FSM_FG finding time is %fms\n", endtime);                                           \
    }
#define getAMD3D_IMPL(T) _getAMD3D_IMPL(T)

getAMD3D_IMPL(int);
getAMD3D_IMPL(float);
getAMD3D_IMPL(double);

///////////////////////  Process stream3D in 2D dataset /////////////////////////
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
        getAMD3D(T)(rows, cols, layers, data, Level_X, Level_X_size);      \
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
        getAMD3D(T)(rows, cols, Layers, data, Level_X, Level_X_size);                 \
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
        getAMD3D(T)(rows, cols, layers, data, Level_X, Level_X_size);     \
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
