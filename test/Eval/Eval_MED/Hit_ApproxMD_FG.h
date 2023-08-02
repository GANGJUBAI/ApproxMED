#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <string.h>
#include <stdbool.h>

#define Eps 1e-15
#define PI 3.14159265358979323846
int Cnt[47];
int findProper=1;
int noFindProper=2;

////////////////////// Scale Function //////////////////
double scaleFuc(double x)
{
    return 5 * PI * atan(2 * x) + 23;
}

/////////////////////////////////////////// Compute Cnt ////////////////////////////////////////
#define _computeCnt(T) computeCnt$_##T##_$
#define computeCnt(T) _computeCnt(T)

#define _computeCnt_IMPL(T)                                                                \
    void computeCnt(T)(T Array[], int groupNum, int dataLength, double mean, double delta) \
    {                                                                                      \
        const double inv_delta = 1.0 / delta;                                              \
        const int upper_bound = groupNum - 1;                                              \
        const double offset_const = mean * inv_delta;                                      \
        double offset;                                                                     \
        int index;                                                                         \
        int *cnt = Cnt;                                                                    \
        T *array = Array;                                                                  \
                                                                                           \
        for (int i = 0; i < dataLength; i++)                                               \
        {                                                                                  \
            offset = array[i] * inv_delta - offset_const;                                  \
            index = (int)scaleFuc(offset);                                                 \
            if (index < 0)                                                                 \
                index = 0;                                                                 \
            if (index > upper_bound)                                                       \
                index = upper_bound;                                                       \
            cnt[index + 1]++;                                                              \
        }                                                                                  \
    }
#define computeCnt_IMPL(T) _computeCnt_IMPL(T)

computeCnt_IMPL(int);
computeCnt_IMPL(float);
computeCnt_IMPL(double);

///////////////////////////////////////  Median Find Stage1 /////////////////////////////////////////////
#define _medianFindStage1(T) medianFindStage1$_##T##_$
#define medianFindStage1(T) _medianFindStage1(T)

#define _medianFindStage1_IMPL(T)                                                                    \
    double medianFindStage1(T)(T * Array, int groupNum, int dataLength, double *mean, double *delta) \
    {                                                                                                \
        double var;                                                                                  \
        double sum;                                                                                  \
        double squareSum;                                                                            \
        double Mean;                                                                                 \
        double Delta;                                                                                \
        int cdfSum = 0;                                                                              \
        int upperLength = dataLength - 1;                                                            \
        int HALF_LENGTH = dataLength / 2;                                                            \
        int HALF_GROUP = groupNum / 2;                                                               \
        const double inv_PI_5 = 1 / (5.0 * PI);                                                      \
        const float half_len = dataLength / 2.0;                                                     \
                                                                                                     \
        memset(Cnt, 0, sizeof(Cnt));                                                                 \
                                                                                                     \
        sum = 0;                                                                                     \
        var = 0;                                                                                     \
        squareSum = 0;                                                                               \
                                                                                                     \
        for (int i = 0; i < dataLength; i++)                                                         \
        {                                                                                            \
            double tempArray = (double)Array[i];                                                     \
            sum += tempArray;                                                                        \
            squareSum += tempArray * tempArray;                                                      \
        }                                                                                            \
                                                                                                     \
        Mean = sum / dataLength;                                                                     \
        var = squareSum / dataLength - Mean * Mean;                                                  \
        Delta = sqrt(var);                                                                           \
                                                                                                     \
        if (-Eps < Delta && Delta < Eps || isnan(Delta))                                             \
        {                                                                                            \
            *delta = 0;                                                                              \
            *mean = Mean;                                                                            \
            return Mean;                                                                             \
        }                                                                                            \
                                                                                                     \
        computeCnt(T)(Array, groupNum, dataLength, Mean, Delta);                                     \
                                                                                                     \
        /*get the approximate median*/                                                               \
        int i;                                                                                       \
        for (i = 0; i < groupNum; i++)                                                               \
        {                                                                                            \
            cdfSum += Cnt[i];                                                                        \
            if (cdfSum >= HALF_LENGTH)                                                               \
                break;                                                                               \
        }                                                                                            \
                                                                                                     \
        int last_cdfSum = cdfSum - Cnt[i];                                                           \
        double lower_arg = (i - 1 - HALF_GROUP) * inv_PI_5;                                          \
        double upper_arg = lower_arg + inv_PI_5;                                                     \
        double half_delta = 0.5 * Delta;                                                             \
        double tan_lower = tan(lower_arg);                                                           \
        double tan_upper = tan(upper_arg);                                                           \
        double lower_bound = Mean + half_delta * tan_lower;                                          \
        double bucket_width = half_delta * (tan_upper - tan_lower);                                  \
        double Median = lower_bound + ((half_len - last_cdfSum) / Cnt[i]) * bucket_width;            \
                                                                                                     \
        *mean = Mean;                                                                                \
        *delta = Delta;                                                                              \
        return Median;                                                                               \
    }
#define medianFindStage1_IMPL(T) _medianFindStage1_IMPL(T)

medianFindStage1_IMPL(int);
medianFindStage1_IMPL(float);
medianFindStage1_IMPL(double);

///////////////////////////////////////  Median Find Stage2 ////////////////////////////////////////////
#define _medianFindStage2(T) medianFindStage2$_##T##_$
#define medianFindStage2(T) _medianFindStage2(T)

#define _medianFindStage2_IMPL(T)                                                                    \
    double medianFindStage2(T)(T * Array, int groupNum, int dataLength, double *mean, double *delta) \
    {                                                                                                \
        double var;                                                                                  \
        double sum;                                                                                  \
        double oriMean;                                                                              \
        double oriDelta;                                                                             \
        double meanTemp;                                                                             \
        double deltaTemp;                                                                            \
        double squareSum;                                                                            \
                                                                                                     \
        int leftsum;                                                                                 \
        int rightsum;                                                                                \
        int adjustIndex;                                                                             \
        int deltaIndex;                                                                              \
        int right = groupNum - 1;                                                                    \
        int HALF_GROUP = groupNum / 2;                                                               \
        int HALF_LENGTH = dataLength / 2;                                                            \
        const double inv_PI_5 = 1 / (5.0 * PI);                                                      \
        const float half_len = dataLength / 2.0;                                                     \
        /*int *Cnt=(int *)calloc(groupNum,sizeof(int));*/                                            \
        memset(Cnt, 0, sizeof(Cnt));                                                                 \
                                                                                                     \
        sum = 0;                                                                                     \
        var = 0;                                                                                     \
        oriMean = *mean;                                                                             \
        oriDelta = *delta;                                                                           \
        squareSum = 0;                                                                               \
        leftsum = 0;                                                                                 \
        rightsum = 0;                                                                                \
        adjustIndex = 0;                                                                             \
        deltaIndex = 0;                                                                              \
        if (-Eps < oriDelta && oriDelta < Eps || isnan(deltaTemp))                                   \
        {                                                                                            \
            deltaIndex = 1;                                                                          \
            adjustIndex = 1;                                                                         \
        }                                                                                            \
        const double inv_delta = 1.0 / oriDelta;                                                     \
        const int upper_bound = groupNum - 1;                                                        \
        const double offset_const = oriMean * inv_delta;                                             \
        double offset;                                                                               \
        int index;                                                                                   \
                                                                                                     \
        if (deltaIndex == 0)                                                                         \
        {                                                                                            \
            for (int i = 0; i < dataLength; i++)                                                     \
            {                                                                                        \
                /* compute location of Arrays of elements in group                                   \
                and compute the count within each group*/                                            \
                double tempArray = (double)Array[i];                                                 \
                                                                                                     \
                offset = tempArray * inv_delta - offset_const;                                       \
                index = (int)scaleFuc(offset);                                                       \
                if (index < 0)                                                                       \
                    index = 0;                                                                       \
                if (index > upper_bound)                                                             \
                    index = upper_bound;                                                             \
                Cnt[index + 1]++;                                                                    \
                                                                                                     \
                sum += tempArray;                                                                    \
                squareSum += tempArray * tempArray;                                                  \
            }                                                                                        \
        }                                                                                            \
        else if (deltaIndex == 1)                                                                    \
        {                                                                                            \
            for (int i = 0; i < dataLength; i++)                                                     \
            {                                                                                        \
                double tempArray = (double)Array[i];                                                 \
                sum += tempArray;                                                                    \
                squareSum += tempArray * tempArray;                                                  \
            }                                                                                        \
        }                                                                                            \
        /*compute mean and delta currently*/                                                         \
        meanTemp = sum / dataLength;                                                                 \
        var = squareSum / dataLength - meanTemp * meanTemp;                                          \
        deltaTemp = sqrt(var);                                                                       \
                                                                                                     \
        int properIndex = 2;                                                                         \
        for (int i = 0; i < properIndex; i++)                                                        \
        {                                                                                            \
            leftsum += Cnt[i];                                                                       \
            if (leftsum >= HALF_LENGTH)                                                              \
            {                                                                                        \
                adjustIndex = 1;                                                                     \
                break;                                                                               \
            }                                                                                        \
            rightsum += Cnt[right--];                                                                \
            if (rightsum >= HALF_LENGTH)                                                             \
            {                                                                                        \
                adjustIndex = 1;                                                                     \
                break;                                                                               \
            }                                                                                        \
        }                                                                                            \
                                                                                                     \
        int last_cdfSum;                                                                             \
        double lower_arg;                                                                            \
        double upper_arg;                                                                            \
        double half_delta;                                                                           \
        double tan_lower;                                                                            \
        double tan_upper;                                                                            \
        double lower_bound;                                                                          \
        double bucket_width;                                                                         \
        double Median;                                                                               \
        if (adjustIndex == 0)                                                                        \
        {                                                                                            \
            printf("is Find Proper Distribution:%d\n",findProper);                                   \
            int cdfSum = 0;                                                                          \
            int i;                                                                                   \
            for (i = 0; i < groupNum; i++)                                                           \
            {                                                                                        \
                cdfSum += Cnt[i];                                                                    \
                if (cdfSum >= HALF_LENGTH)                                                           \
                    break;                                                                           \
            }                                                                                        \
                                                                                                     \
            *mean = meanTemp;                                                                        \
            *delta = deltaTemp;                                                                      \
                                                                                                     \
            last_cdfSum = cdfSum - Cnt[i];                                                           \
            lower_arg = (i - 1 - HALF_GROUP) * inv_PI_5;                                             \
            upper_arg = lower_arg + inv_PI_5;                                                        \
            half_delta = 0.5 * oriDelta;                                                             \
            tan_lower = tan(lower_arg);                                                              \
            tan_upper = tan(upper_arg);                                                              \
            lower_bound = oriMean + half_delta * tan_lower;                                          \
            bucket_width = half_delta * (tan_upper - tan_lower);                                     \
            Median = lower_bound + ((half_len - last_cdfSum) / Cnt[i]) * bucket_width;               \
            return Median;                                                                           \
        }                                                                                            \
                                                                                                     \
        if (adjustIndex == 1)                                                                        \
        {                                                                                            \
            printf("is Find Proper Distribution:%d\n",noFindProper);                                 \
            memset(Cnt, 0, sizeof(Cnt));                                                             \
            *mean = meanTemp;                                                                        \
            *delta = deltaTemp;                                                                      \
                                                                                                     \
            if (-Eps < deltaTemp && deltaTemp < Eps || isnan(deltaTemp))                             \
            {                                                                                        \
                *delta = 0;                                                                          \
                return meanTemp;                                                                     \
            }                                                                                        \
            else                                                                                     \
                computeCnt(T)(Array, groupNum, dataLength, meanTemp, deltaTemp);                     \
                                                                                                     \
            int cdfSum = 0;                                                                          \
            int i;                                                                                   \
            for (i = 0; i < groupNum; i++)                                                           \
            {                                                                                        \
                cdfSum += Cnt[i];                                                                    \
                if (cdfSum >= HALF_LENGTH)                                                           \
                    break;                                                                           \
            }                                                                                        \
            last_cdfSum = cdfSum - Cnt[i];                                                           \
            lower_arg = (i - 1 - HALF_GROUP) * inv_PI_5;                                             \
            upper_arg = lower_arg + inv_PI_5;                                                        \
            half_delta = 0.5 * deltaTemp;                                                            \
            tan_lower = tan(lower_arg);                                                              \
            tan_upper = tan(upper_arg);                                                              \
            lower_bound = meanTemp + half_delta * tan_lower;                                         \
            bucket_width = half_delta * (tan_upper - tan_lower);                                     \
            Median = lower_bound + ((half_len - last_cdfSum) / Cnt[i]) * bucket_width;               \
                                                                                                     \
            return Median;                                                                           \
        }                                                                                            \
    }
#define medianFindStage2_IMPL(T) _medianFindStage2_IMPL(T)

medianFindStage2_IMPL(int);
medianFindStage2_IMPL(float);
medianFindStage2_IMPL(double);
