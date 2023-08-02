#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <string.h>

#define Eps 1e-15
int Cnt[8]; 

/////////////////////////////////////////// Compute Cnt /////////////////////////////////////////////////
#define _computeCnt(T) computeCnt$_##T##_$
#define computeCnt(T) _computeCnt(T)

#define _computeCnt_IMPL(T)                                                                             \
void computeCnt(T)(T Array[], int groupNum, int dataLength, double mean, double delta,int* cmpCnt) { 	\
	int cmp_count=0;										\
	for(int i=0;i<dataLength;i++)									\
	{												\
		cmp_count++;	      									\
													\
		double offset=(Array[i]-mean)/delta;							\
		if(offset<-3) Cnt[0]++;									\
		else if (offset>3) Cnt[groupNum-1]++;							\
		else Cnt[ (int) (offset+4)]++;								\
	}												\
	*cmpCnt += cmp_count;										\
}
#define computeCnt_IMPL(T) _computeCnt_IMPL(T)

computeCnt_IMPL(int);
computeCnt_IMPL(float);
computeCnt_IMPL(double);


///////////////////////////////////////  Median Find Stage1 /////////////////////////////////////////////
#define _medianFindStage1(T) medianFindStage1$_##T##_$
#define medianFindStage1(T) _medianFindStage1(T)

#define _medianFindStage1_IMPL(T)                                        				\
double medianFindStage1(T)(T *Array,int groupNum,int dataLength,double *mean,double *delta,int* cmpCnt){	\
	double var;											\
	double sum;											\
	double squareSum;										\
	double Mean;											\
	double Delta;											\
	int cdfSum=0;											\
	/*int *Cnt=(int *)calloc(groupNum,sizeof(int));*/							\
	memset(Cnt,0,sizeof(Cnt));\
													\
	sum=0;												\
	var=0;												\
	squareSum=0;											\
													\
	for(int i=0;i< dataLength;i++){									\
	  sum += (double)Array[i];									\
	  squareSum +=(double)Array[i]*Array[i];							\
	}												\
													\
	Mean = sum/dataLength;										\
	var = squareSum/dataLength-Mean*Mean;								\
	/*printf("squareSum is %.20f\n",squareSum);*/\
	/*printf("var is %lf\n",var);*/\
	Delta = sqrt(var);										\
	if(-Eps < Delta && Delta <Eps){									\
      		/*free(Cnt);*/										\
		*delta =0;										\
      		return Mean;										\
	}												\
													\
	computeCnt(T)(Array,groupNum,dataLength,Mean,Delta,cmpCnt);						\
													\
	int i;												\
	for(i=0;i<groupNum;i++)										\
	{												\
		cdfSum+=Cnt[i];										\
		if(cdfSum>=dataLength/2)								\
		break;											\
	}												\
													\
	/*free(Cnt);*/											\
	*mean= Mean;											\
	*delta =Delta;											\
	return Mean+(i-3.5)*(Delta);									\
}
#define medianFindStage1_IMPL(T) _medianFindStage1_IMPL(T)

medianFindStage1_IMPL(int);
medianFindStage1_IMPL(float);
medianFindStage1_IMPL(double);


///////////////////////////////////////  Median Find Stage2 ////////////////////////////////////////////
#define _medianFindStage2(T) medianFindStage2$_##T##_$
#define medianFindStage2(T) _medianFindStage2(T)

#define _medianFindStage2_IMPL(T)									\
double medianFindStage2(T)(T *Array,int groupNum,int dataLength,double *mean,double *delta, int* cmpCnt){	\
  double var;												\
  double sum;												\
  double oriMean;											\
  double oriDelta;											\
  double meanTemp;											\
  double deltaTemp;											\
  double squareSum;											\
  													\
  int groupLoc;												\
  int leftsum;												\
  int rightsum;												\
  int adjustIndex;											\
  int deltaIndex;											\
  int right = groupNum-1;										\
  int HALF_GROUP = groupNum/2-2;									\
  int HALF_LENGTH =dataLength/2;									\
  /*int *Cnt=(int *)calloc(groupNum,sizeof(int));*/							\
  memset(Cnt,0,sizeof(Cnt));\
  int cmp_count=0;											\
													\
  sum=0;												\
  var=0;												\
  oriMean=*mean;											\
  oriDelta=*delta;											\
  squareSum=0;												\
  leftsum=0;												\
  rightsum=0;												\
  adjustIndex=0;											\
  deltaIndex=0;												\
  if(-Eps <oriDelta && oriDelta <Eps){									\
    deltaIndex=1;											\
    adjustIndex=1;											\
  }													\
													\
  if(deltaIndex == 0){											\
	  for(int i=0;i< dataLength;i++){								\
	    cmp_count++;										\
		  											\
	    double offset=(Array[i]-oriMean)/oriDelta;							\
	    if(offset<-3) Cnt[0]++;									\
	    else if (offset>3) Cnt[groupNum-1]++;							\
	    else Cnt[ (int) (offset+4)]++;								\
													\
	    sum +=(double)Array[i];									\
	    squareSum +=(double)Array[i]*Array[i];							\
	  }												\
  }													\
  else if(deltaIndex == 1){										\
    for(int i=0;i< dataLength;i++){									\
	    cmp_count++;										\
	    												\
            sum +=(double)Array[i];									\
            squareSum +=(double)Array[i]*Array[i];							\
    }													\
  }													\
 													\
  *cmpCnt += cmp_count;											\
													\
  meanTemp= sum/dataLength;										\
  var= squareSum/dataLength-meanTemp*meanTemp;								\
  deltaTemp=sqrt(var);											\
													\
  for(int i=0;i<HALF_GROUP;i++){									\
    leftsum +=Cnt[i];											\
    if(leftsum >=HALF_LENGTH){										\
      adjustIndex=1;											\
      break;												\
    }													\
    rightsum +=Cnt[right--];										\
    if(rightsum >=HALF_LENGTH){										\
      adjustIndex=1;											\
      break;												\
    }													\
  }													\
													\
   if(adjustIndex == 0){										\
	    int cdfSum=0;										\
	    int i;											\
	    for(i=0;i<groupNum;i++)									\
	    {												\
		  cdfSum+=Cnt[i];									\
		  if(cdfSum>=dataLength/2) break;							\
	    }												\
													\
	    /*free(Cnt);*/											\
	    *mean = meanTemp;										\
	    *delta = deltaTemp;										\
	    return oriMean+(i-3.5)*oriDelta;								\
  }													\
  													\
   if(adjustIndex ==1){											\
	    memset(Cnt,0,sizeof(Cnt));									\
	    *mean=meanTemp;										\
	    *delta=deltaTemp;										\
													\
	   if(-Eps < deltaTemp && deltaTemp <Eps){							\
	     /* free(Cnt);*/										\
	      *delta=0;											\
	      return meanTemp;										\
	    }												\
	   else computeCnt(T)(Array,groupNum,dataLength,meanTemp,deltaTemp,cmpCnt);				\
													\
	    int cdfSum=0;										\
	    int i;											\
	    for(i=0;i<groupNum;i++)									\
	    {												\
		      cdfSum+=Cnt[i];									\
		      if(cdfSum>=dataLength/2) break;							\
	    }												\
	    /*free(Cnt);*/											\
	    return meanTemp+(i-3.5)*deltaTemp;								\
   }													\
}
#define medianFindStage2_IMPL(T) _medianFindStage2_IMPL(T)

medianFindStage2_IMPL(int);
medianFindStage2_IMPL(float);
medianFindStage2_IMPL(double);

