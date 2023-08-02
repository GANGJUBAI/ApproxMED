// Main_FSM_ori.c
// Author:Fenfang Li
// Time:2023/3

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "CAS_ApproxMD.h"

static int cmp_count = 0;
int unused __attribute__((unused));

////////////////////////////////////////////// Get FSM Approximate Median 1D ////////////////////////////////////////////
#define _getAMD1D(T) getAMD1D$_##T##_$
#define getAMD1D(T) _getAMD1D(T)

#define _getAMD1D_IMPL(T)                                                                                       \
    void getAMD1D(T)(int rows, T *data, int Level_X_size)                                                       \
    {                                                                                                           \
        struct timeval start, end;                                                                              \
        double mean = 0, median = 0, sigma = 0;                                                                 \
        int findMedianStage = 0;                                                                                \
                                                                                                                \
        gettimeofday(&start, NULL);                                                                             \
        for (int i = 0; i < rows; i++)                                                                          \
        {                                                                                                       \
            switch (findMedianStage)                                                                            \
            {                                                                                                   \
            case 0:                                                                                             \
                median = medianFindStage1(T)(data, 8, Level_X_size, &mean, &sigma, &cmp_count);                 \
                findMedianStage = 1;                                                                            \
                break;                                                                                          \
            case 1:                                                                                             \
                median = medianFindStage2(T)(data, 8, Level_X_size, &mean, &sigma, &cmp_count);                 \
                break;                                                                                          \
            }                                                                                                   \
        }                                                                                                       \
        gettimeofday(&end, NULL);                                                                               \
        double endtime = (double)1000 * (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000.0); \
        /*printf("Approximate_FSM_ori finding time is %fms\n",endtime);           */                            \
    }
#define getAMD1D_IMPL(T) _getAMD1D_IMPL(T)

getAMD1D_IMPL(int);
getAMD1D_IMPL(float);
getAMD1D_IMPL(double);

//////////////////////////////  Process stream 1D  //////////////////////////////
#define _process_stream1D(T) process_stream1D$_##T##_$
#define process_stream1D(T) _process_stream1D(T)

#define _process_stream1D_IMPL(T)                             \
    int process_stream1D(T)(int rows, int cols, FILE *ori_fp) \
    {                                                         \
        int dataLength = cols;                                \
        int Level_X_size = cols;                              \
                                                              \
        T *data = (T *)malloc(dataLength * sizeof(T));        \
        unused = fread(data, dataLength, sizeof(T), ori_fp);  \
        fclose(ori_fp);                                       \
        ori_fp = NULL;                                        \
                                                              \
        getAMD1D(T)(rows, data, Level_X_size);                \
                                                              \
        printf("T is int :%ld\n", sizeof(T));                 \
        free(data);                                           \
        data = NULL;                                          \
        return 0;                                             \
    }
#define process_stream1D_IMPL(T) _process_stream1D_IMPL(T)

process_stream1D_IMPL(int);
process_stream1D_IMPL(float);
process_stream1D_IMPL(double);

////////////////////////////////////////////// Get Approximate Median 2D ////////////////////////////////////////////////
#define _getAMD2D(T) getAMD2D$_##T##_$
#define getAMD2D(T) _getAMD2D(T)

#define _getAMD2D_IMPL(T)                                                                                               \
    void getAMD2D(T)(int rows, int cols, int layers, T *data, T *Level_X, int Level_X_size)                             \
    {                                                                                                                   \
        struct timeval start, end;                                                                                      \
        double mean = 0, median = 0, sigma = 0;                                                                         \
        int findMedianStage = 0;                                                                                        \
                                                                                                                        \
        gettimeofday(&start, NULL);                                                                                     \
        for (int k = 0; k < layers; k++)                                                                                \
        {                                                                                                               \
            for (int i = 0; i < rows; i++)                                                                              \
            {                                                                                                           \
                for (int j = 0; j < cols; j++)                                                                          \
                {                                                                                                       \
                    Level_X[i * cols + j] = data[k * rows * cols + i * cols + j];                                       \
                }                                                                                                       \
            }                                                                                                           \
            switch (findMedianStage)                                                                                    \
            {                                                                                                           \
            case 0:                                                                                                     \
                median = medianFindStage1(T)(Level_X, 8, Level_X_size, &mean, &sigma, &cmp_count);                      \
                findMedianStage = 1;                                                                                    \
                break;                                                                                                  \
            case 1:                                                                                                     \
                median = medianFindStage2(T)(Level_X, 8, Level_X_size, &mean, &sigma, &cmp_count);                      \
                break;                                                                                                  \
            }                                                                                                           \
        }                                                                                                               \
        gettimeofday(&end, NULL);                                                                                       \
        double endtime = (double)1000 * (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000.0);         \
        /*printf("qselect finding time is %fms\n",endtime);                                                          */ \
    }
#define getAMD2D_IMPL(T) _getAMD2D_IMPL(T)

getAMD2D_IMPL(int);
getAMD2D_IMPL(float);
getAMD2D_IMPL(double);

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
        unused = fread(data, dataLength, sizeof(T), ori_fp);              \
        fclose(ori_fp);                                                   \
        ori_fp = NULL;                                                    \
        T *Level_X = (T *)malloc(Level_X_size * sizeof(T));               \
                                                                          \
        getAMD2D(T)(rows, cols, layers, data, Level_X, Level_X_size);     \
                                                                          \
        printf("T is int :%ld\n", sizeof(T));                             \
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
                    median = medianFindStage1(T)(Level_X, 8, Level_X_size, &mean, &sigma, &cmp_count);          \
                    findMedianStage = 1;                                                                        \
                    break;                                                                                      \
                case 1:                                                                                         \
                    median = medianFindStage2(T)(Level_X, 8, Level_X_size, &mean, &sigma, &cmp_count);          \
                    break;                                                                                      \
                }                                                                                               \
            }                                                                                                   \
        }                                                                                                       \
        gettimeofday(&end, NULL);                                                                               \
        double endtime = (double)1000 * (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000.0); \
        /*printf("Approximate_FSM_ori finding time is %fms\n",endtime);			*/                              \
    }
#define getAMD3D_IMPL(T) _getAMD3D_IMPL(T)

getAMD3D_IMPL(int);
getAMD3D_IMPL(float);
getAMD3D_IMPL(double);

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
        unused = fread(data, dataLength, sizeof(T), ori_fp);              \
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

int main(int argc, char *argv[])
{

    char oriFilePath[640];
    char *dataType = NULL;
    int rows, cols, layers, dim;

    if (argc < 6)
    {
        printf("Test case: Main [srcFilePath] [dimension sizes...] [data type] [dimension num]\n");
        printf("Example: Main data.dat 8 8 [128] I(int)|F(float)|D(double) 1|2|3\n");
        exit(0);
    }

    sprintf(oriFilePath, "%s", argv[1]);
    rows = atoi(argv[2]);
    cols = atoi(argv[3]);
    if (argc == 6)
    {
        dataType = argv[4];
        dim = atoi(argv[5]);
    }
    if (argc == 7)
    {
        layers = atoi(argv[4]);
        dataType = argv[5];
        dim = atoi(argv[6]);
    }

    FILE *ori_fp = fopen(oriFilePath, "r");
    if (ori_fp == NULL)
    {
        printf("Cannot open file %s.\n", oriFilePath);
    }
    switch (*dataType)
    {
    case 'I':
        switch (dim)
        {
        case 1:
            process_stream1D(int)(rows, cols, ori_fp);
            break;
        case 2:
            process_stream2D(int)(rows, cols, layers, ori_fp);
            break;
        }
    case 'F':
        switch (dim)
        {
        case 2:
            process_stream2D(float)(rows, cols, layers, ori_fp);
            break;
        case 3:
            process_stream3D(float)(rows, cols, layers, ori_fp);
            break;
        }
    case 'D':
        switch (dim)
        {
        case 2:
            process_stream2D(double)(rows, cols, layers, ori_fp);
            break;
        case 3:
            process_stream3D(double)(rows, cols, layers, ori_fp);
            break;
        }

    default:
        break;
    }

    printf("comp_count is %d\n", cmp_count);
}
