#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <string.h>

#define Eps 1e-15
int Cnt[8];
int findProper=1;
int noFindProper=2;


/* safe min & max */
/* avoid ++/-- expand twice */
/* compile warning if type diff */
#define min(x, y) ({                            \
        typeof(x) _min1 = (x);                  \
        typeof(y) _min2 = (y);                  \
        (void) (&_min1 == &_min2);              \
        _min1 < _min2 ? _min1 : _min2; })

#define max(x, y) ({                            \
        typeof(x) _max1 = (x);                  \
        typeof(y) _max2 = (y);                  \
        (void) (&_max1 == &_max2);              \
        _max1 > _max2 ? _max1 : _max2; })

/////////////////////////////////////////// Compute Cnt /////////////////////////////////////////////////
#define _computeCnt(T) computeCnt$_##T##_$
#define computeCnt(T) _computeCnt(T)

#define _computeCnt_IMPL(T)                                                              \
  void computeCnt(T)(T Array[], int groupNum, int dataLength, double mean, double delta) \
  {                                                                                      \
    const double inv_delta = 1.0 / delta;                                                \
    const int upper_bound = groupNum - 1;                                                \
    const double offset_const = mean * inv_delta;                                        \
    int *cnt = Cnt;                                                                      \
    T *array = Array;                                                                    \
                                                                                         \
    for (int i = 0; i < dataLength; i++)                                                 \
    {                                                                                    \
      double offset = array[i] * inv_delta - offset_const;                               \
      int bin = (int)((offset + 4) / 1);                                                 \
      bin = max(0, min(bin, upper_bound));                                               \
      cnt[bin]++;                                                                        \
    }                                                                                    \
  }
#define computeCnt_IMPL(T) _computeCnt_IMPL(T)

computeCnt_IMPL(int);
computeCnt_IMPL(float);
computeCnt_IMPL(double);

///////////////////////////////////////  Median Find Stage1 /////////////////////////////////////////////
#define _medianFindStage1(T) medianFindStage1$_##T##_$
#define medianFindStage1(T) _medianFindStage1(T)

#define _medianFindStage1_IMPL(T)                                                                  \
  double medianFindStage1(T)(T * Array, int groupNum, int dataLength, double *mean, double *delta) \
  {                                                                                                \
    double var;                                                                                    \
    double sum;                                                                                    \
    double squareSum;                                                                              \
    double Mean;                                                                                   \
    double Delta;                                                                                  \
    int cdfSum = 0;                                                                                \
    int upperLength = dataLength - 1;                                                              \
    int HALF_LENGTH = dataLength / 2;                                                              \
                                                                                                   \
    memset(Cnt, 0, sizeof(Cnt));                                                                   \
                                                                                                   \
    sum = 0;                                                                                       \
    var = 0;                                                                                       \
    squareSum = 0;                                                                                 \
                                                                                                   \
    for (int i = 0; i < dataLength; i++)                                                           \
    {                                                                                              \
      double tempArray = (double)Array[i];                                                         \
      sum += tempArray;                                                                            \
      squareSum += tempArray * tempArray;                                                          \
    }                                                                                              \
                                                                                                   \
    Mean = sum / dataLength;                                                                       \
    var = squareSum / dataLength - Mean * Mean;                                                    \
    Delta = sqrt(var);                                                                             \
                                                                                                   \
    if (-Eps < Delta && Delta < Eps || isnan(Delta))                                               \
    {                                                                                              \
      *delta = 0;                                                                                  \
      *mean = Mean;                                                                                \
      return Mean;                                                                                 \
    }                                                                                              \
                                                                                                   \
    computeCnt(T)(Array, groupNum, dataLength, Mean, Delta);                                       \
                                                                                                   \
    /*get the approximate median*/                                                                 \
    int i;                                                                                         \
    for (i = 0; i < groupNum; i++)                                                                 \
    {                                                                                              \
      cdfSum += Cnt[i];                                                                            \
      if (cdfSum >= HALF_LENGTH)                                                                   \
        break;                                                                                     \
    }                                                                                              \
    int last_cdfSum = cdfSum - Cnt[i];                                                             \
    double L1 = Mean + (i - 4) * Delta;                                                            \
    double Median = L1 + ((dataLength / 2.0 - last_cdfSum) / Cnt[i]) * Delta;                      \
                                                                                                   \
    *mean = Mean;                                                                                  \
    *delta = Delta;                                                                                \
    return Median;                                                                                 \
  }
#define medianFindStage1_IMPL(T) _medianFindStage1_IMPL(T)

medianFindStage1_IMPL(int);
medianFindStage1_IMPL(float);
medianFindStage1_IMPL(double);

///////////////////////////////////////  Median Find Stage2 ////////////////////////////////////////////
#define _medianFindStage2(T) medianFindStage2$_##T##_$
#define medianFindStage2(T) _medianFindStage2(T)

#define _medianFindStage2_IMPL(T)                                                                  \
  double medianFindStage2(T)(T * Array, int groupNum, int dataLength, double *mean, double *delta) \
  {                                                                                                \
    double var;                                                                                    \
    double sum;                                                                                    \
    double oriMean;                                                                                \
    double oriDelta;                                                                               \
    double meanTemp;                                                                               \
    double deltaTemp;                                                                              \
    double squareSum;                                                                              \
    double L1;                                                                                     \
    double Median;                                                                                 \
                                                                                                   \
    int groupLoc;                                                                                  \
    int leftsum;                                                                                   \
    int rightsum;                                                                                  \
    int last_cdfSum;                                                                               \
    int adjustIndex;                                                                               \
    int deltaIndex;                                                                                \
    int right = groupNum - 1;                                                                      \
    int HALF_GROUP = groupNum / 2 - 2;                                                             \
    int HALF_LENGTH = dataLength / 2;                                                              \
    /*int *Cnt=(int *)calloc(groupNum,sizeof(int));*/                                              \
    memset(Cnt, 0, sizeof(Cnt));                                                                   \
                                                                                                   \
    sum = 0;                                                                                       \
    var = 0;                                                                                       \
    oriMean = *mean;                                                                               \
    oriDelta = *delta;                                                                             \
    squareSum = 0;                                                                                 \
    leftsum = 0;                                                                                   \
    rightsum = 0;                                                                                  \
    adjustIndex = 0;                                                                               \
    deltaIndex = 0;                                                                                \
    if (-Eps < oriDelta && oriDelta < Eps || isnan(oriDelta))                                      \
    {                                                                                              \
      deltaIndex = 1;                                                                              \
      adjustIndex = 1;                                                                             \
    }                                                                                              \
    const double inv_delta = 1.0 / oriDelta;                                                       \
    const int upper_bound = groupNum - 1;                                                          \
    const double offset_const = oriMean * inv_delta;                                               \
                                                                                                   \
    if (deltaIndex == 0)                                                                           \
    {                                                                                              \
      for (int i = 0; i < dataLength; i++)                                                         \
      {                                                                                            \
        /* compute location of Arrays of elements in group                                         \
        and compute the count within each group*/                                                  \
        double tempArray = (double)Array[i];                                                       \
                                                                                                   \
        double offset = tempArray * inv_delta - offset_const;                                      \
        int bin = (int)((offset + 4) / 1);                                                         \
        bin = max(0, min(bin, upper_bound));                                                       \
        Cnt[bin]++;                                                                                \
                                                                                                   \
        sum += tempArray;                                                                          \
        squareSum += tempArray * tempArray;                                                        \
      }                                                                                            \
    }                                                                                              \
    else if (deltaIndex == 1)                                                                      \
    {                                                                                              \
      for (int i = 0; i < dataLength; i++)                                                         \
      {                                                                                            \
        double tempArray = (double)Array[i];                                                       \
        sum += tempArray;                                                                          \
        squareSum += tempArray * tempArray;                                                        \
      }                                                                                            \
    }                                                                                              \
    /*compute mean and delta currently*/                                                           \
    meanTemp = sum / dataLength;                                                                   \
    var = squareSum / dataLength - meanTemp * meanTemp;                                            \
    deltaTemp = sqrt(var);                                                                         \
                                                                                                   \                                                                                         \
                                                                                                   \
    for (int i = 0; i < HALF_GROUP; i++)                                                           \
    {                                                                                              \
      leftsum += Cnt[i];                                                                           \
      if (leftsum >= HALF_LENGTH)                                                                  \
      {                                                                                            \
        adjustIndex = 1;                                                                           \
        break;                                                                                     \
      }                                                                                            \
      rightsum += Cnt[right--];                                                                    \
      if (rightsum >= HALF_LENGTH)                                                                 \
      {                                                                                            \
        adjustIndex = 1;                                                                           \
        break;                                                                                     \
      }                                                                                            \
    }                                                                                              \
                                                                                                   \
    if (adjustIndex == 0)                                                                          \
    {                                                                                              \
      printf("is Find Proper Distribution:%d\n",findProper);                                       \
      int cdfSum = 0;                                                                              \
      int i;                                                                                       \
      for (i = 0; i < groupNum; i++)                                                               \
      {                                                                                            \
        cdfSum += Cnt[i];                                                                          \
        if (cdfSum >= HALF_LENGTH)                                                                 \
          break;                                                                                   \
      }                                                                                            \
      last_cdfSum = cdfSum - Cnt[i];                                                               \
      L1 = oriMean + (i - 4) * oriDelta;                                                           \
      Median = L1 + ((dataLength / 2.0 - last_cdfSum) / Cnt[i]) * oriDelta;                        \
                                                                                                   \
      *mean = meanTemp;                                                                            \
      *delta = deltaTemp;                                                                          \
      return Median;                                                                               \
    }                                                                                              \
                                                                                                   \
    if (adjustIndex == 1)                                                                          \
    {                                                                                              \
      printf("is Find Proper Distribution:%d\n",noFindProper);                                     \
      memset(Cnt, 0, sizeof(Cnt));                                                                 \
      *mean = meanTemp;                                                                            \
      *delta = deltaTemp;                                                                          \
                                                                                                   \
      if (-Eps < deltaTemp && deltaTemp < Eps ||isnan(deltaTemp))                                  \
      {                                                                                            \
        *delta = 0;                                                                                \
        return meanTemp;                                                                           \
      }                                                                                            \
      else                                                                                         \
        computeCnt(T)(Array, groupNum, dataLength, meanTemp, deltaTemp);                           \
                                                                                                   \
      int cdfSum = 0;                                                                              \
      int i;                                                                                       \
      for (i = 0; i < groupNum; i++)                                                               \
      {                                                                                            \
        cdfSum += Cnt[i];                                                                          \
        if (cdfSum >= HALF_LENGTH)                                                                 \
          break;                                                                                   \
      }                                                                                            \
      last_cdfSum = cdfSum - Cnt[i];                                                               \
      L1 = meanTemp + (i - 4) * deltaTemp;                                                         \
      Median = L1 + ((dataLength / 2.0 - last_cdfSum) / Cnt[i]) * deltaTemp;                       \
      return Median;                                                                               \
    }                                                                                              \
  }
#define medianFindStage2_IMPL(T) _medianFindStage2_IMPL(T)

medianFindStage2_IMPL(int);
medianFindStage2_IMPL(float);
medianFindStage2_IMPL(double);
