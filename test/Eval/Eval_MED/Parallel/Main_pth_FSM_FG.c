// Main_pth_FSM_FG.c
// Author:Fenfang Li
// Time:2023/7

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "ApproxMD_FG.h"
#include <pthread.h>

int rows, cols, layers;
double *data_d;
float *data_f;
int thread_count;

#define __type_is_float(expr) __builtin_types_compatible_p(typeof(expr), float)
#define __type_is_double(expr) __builtin_types_compatible_p(typeof(expr), double)

#define _MedianFinding(T) MedianFinding$_##T##_$
#define MedianFinding(T) _MedianFinding(T)

#define _MedianFinding_IMPL(T)                                                   \
    void *MedianFinding(T)(void *rank)                                           \
    {                                                                            \
        long my_rank = (long)rank;                                               \
        int i, j, k;                                                             \
        double my_mean = 0, my_sigma = 0;                                        \
        double mediantemp = 0;                                                   \
        int findMedianStage = 0;                                                 \
        int local_r = layers / thread_count;                                     \
        int my_first_layer = my_rank * local_r;                                  \
        int my_last_layer = (my_rank + 1) * local_r;                             \
        T *Level_X = (T *)malloc(cols * sizeof(T));                              \
                                                                                 \
        for (k = my_first_layer; k < my_last_layer; k++)                         \
        {                                                                        \
            for (i = 0; i < rows; i++)                                           \
            {                                                                    \
                for (j = 0; j < cols; j++)                                       \
                {                                                                \
                    if (__type_is_float(T))                                      \
                    {                                                            \
                        Level_X[j] = data_f[k * rows * cols + i * cols + j];     \
                    }                                                            \
                    else if (__type_is_double(T))                                \
                    {                                                            \
                        Level_X[j] = data_d[k * rows * cols + i * cols + j];     \
                    }                                                            \
                }                                                                \
                switch (findMedianStage)                                         \
                {                                                                \
                case 0:                                                          \
                    medianFindStage1(T)(Level_X, 46, cols, &my_mean, &my_sigma); \
                    findMedianStage = 1;                                         \
                    break;                                                       \
                case 1:                                                          \
                    medianFindStage2(T)(Level_X, 46, cols, &my_mean, &my_sigma); \
                    break;                                                       \
                }                                                                \
            }                                                                    \
        }                                                                        \
        free(Level_X);                                                           \
        Level_X = NULL;                                                          \
        return NULL;                                                             \
    }
#define MedianFinding_IMPL(T) _MedianFinding_IMPL(T)

MedianFinding_IMPL(float);
MedianFinding_IMPL(double);

#define _process_stream(T) process_stream$_##T##_$
#define process_stream(T) _process_stream(T)

#define _process_stream_IMPL(T)                                                                                 \
    int process_stream(T)(int dataLength, FILE *ori_fp)                                                         \
    {                                                                                                           \
        struct timeval start, end;                                                                              \
        if (__type_is_float(T))                                                                                 \
        {                                                                                                       \
            data_f = (float *)calloc(dataLength, sizeof(float));                                                \
            size_t result = fread(data_f, dataLength, sizeof(float), ori_fp);                                   \
        }                                                                                                       \
        else if (__type_is_double(T))                                                                           \
        {                                                                                                       \
            data_d = (double *)calloc(dataLength, sizeof(double));                                              \
            size_t result = fread(data_d, dataLength, sizeof(double), ori_fp);                                  \
        }                                                                                                       \
                                                                                                                \
        pthread_t *thread_handles;                                                                              \
        long thread;                                                                                            \
        thread_handles = (pthread_t *)malloc(thread_count * sizeof(pthread_t));                                 \
                                                                                                                \
        gettimeofday(&start, NULL);                                                                             \
        for (thread = 0; thread < thread_count; thread++)                                                       \
        {                                                                                                       \
            pthread_create(&thread_handles[thread], NULL, MedianFinding(T), (void *)thread);                    \
        }                                                                                                       \
        for (thread = 0; thread < thread_count; thread++)                                                       \
        {                                                                                                       \
            pthread_join(thread_handles[thread], NULL);                                                         \
        }                                                                                                       \
        gettimeofday(&end, NULL);                                                                               \
        double endtime = (double)1000 * (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000.0); \
        printf("Approximate FG finding time is %fms\n", endtime);                                               \
                                                                                                                \
        fclose(ori_fp);                                                                                         \
        ori_fp = NULL;                                                                                          \
                                                                                                                \
        if (__type_is_float(T))                                                                                 \
        {                                                                                                       \
            free(data_f);                                                                                       \
            data_f = NULL;                                                                                      \
        }                                                                                                       \
        if (__type_is_double(T))                                                                                \
        {                                                                                                       \
            free(data_d);                                                                                       \
            data_d = NULL;                                                                                      \
        }                                                                                                       \
        free(thread_handles);                                                                                   \
        return 0;                                                                                               \
    }
#define process_stream_IMPL(T) _process_stream_IMPL(T)

process_stream_IMPL(float);
process_stream_IMPL(double);

int main(int argc, char *argv[])
{

    char oriFilePath[640];
    char *dataType = NULL;
    int dataLength;

    if (argc < 6)
    {
        printf("Test case: Main [srcFilePath] [dimension sizes...][data type] [threadCount]\n");
        printf("Example: Main data.dat 8 8 128 I(int)|F(float)|D(double) 1\n");
        exit(0);
    }

    sprintf(oriFilePath, "%s", argv[1]);
    rows = atoi(argv[2]);
    cols = atoi(argv[3]);
    layers = atoi(argv[4]);
    dataType = argv[5];
    thread_count = atoi(argv[6]);

    dataLength = rows * cols * layers;
    FILE *ori_fp = fopen(oriFilePath, "r");
    if (ori_fp == NULL)
    {
        printf("Cannot open file %s.\n", oriFilePath);
        return 0;
    }

    switch (*dataType)
    {
    case 'F':
        return process_stream(float)(dataLength, ori_fp);
    case 'D':
        return process_stream(double)(dataLength, ori_fp);
    default:
        break;
    }
}
