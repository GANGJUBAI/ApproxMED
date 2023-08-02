//Main_qsort.c
//Author:Fenfang L
//Time:2023/3

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/time.h>

/* record the number of comparsion and swap */
static int cmp_count = 0;
static int swap_count = 0;

#define __type_is_int(expr) __builtin_types_compatible_p(typeof(expr), int)

#define _cmpfunc(T) cmpfunc$_##T##_$
#define cmpfunc(T) _cmpfunc(T)

#define _cmpfunc_IMPL(T)			\
int cmpfunc(T)(const void * a, const void * b){	\
	cmp_count++;				\
						\
	T res = *(T*)a - *(T*)b;		\
	if(__type_is_int(T)){		\
		if(res >=0) swap_count++;	\
		return res; 			\
	}					\
	else {					\
		if(res >0.0) swap_count++;      \
		return(res > 0.0 ? 1 : -1);	\
	}					\
}
#define cmpfunc_IMPL(T) _cmpfunc_IMPL(T)
cmpfunc_IMPL(int);
cmpfunc_IMPL(float);
cmpfunc_IMPL(double);

////////////////////////////////////////////// Get qsort Exact Median 1D /////////////////////////////////////////////
#define _getQsort1D(T) getQsort1D$_##T##_$
#define getQsort1D(T) _getQsort1D(T)

#define _getQsort1D_IMPL(T)                                                                                           \
void getQsort1D(T)(int rows, T *data, int Level_X_size)                                                               \
{                                                                                                                       \
        struct timeval start, end;                                                                                      \
        double median=0;                                                                                                \
        const size_t m = Level_X_size/2-1;                                                                                \
                                                                                                                        \
        gettimeofday(&start,NULL);                                                                                      \
        for(int i=0; i< rows; i++){                                                                                     \
		qsort(data, Level_X_size, sizeof(T), cmpfunc(T));								\
                median=data[m];                                                                                         \
        }                                                                                                               \
        gettimeofday(&end,NULL);                                                                                        \
        double endtime=(double)1000 * (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000.0);           \
        /*printf("qsort finding time is %fms\n",endtime);                                                    */           \
}
#define getQsort1D_IMPL(T) _getQsort1D_IMPL(T)

getQsort1D_IMPL(int);
getQsort1D_IMPL(float);
getQsort1D_IMPL(double);


////////////////////////////////  Process stream 1D  //////////////////////////////
#define _process_stream1D(T) process_stream1D$_##T##_$
#define process_stream1D(T) _process_stream1D(T)

#define _process_stream1D_IMPL(T)                                               \
int process_stream1D(T)(int rows, int cols,  FILE *ori_fp)                      \
{                                                                               \
        int dataLength = cols;                                          \
        int Level_X_size= cols;                                                 \
                                                                                \
        T *data =(T *)malloc(dataLength*sizeof(T));                             \
        fread(data,dataLength,sizeof(T),ori_fp);                                \
        fclose(ori_fp);                                                         \
        ori_fp = NULL;                                                          \
                                                                                \
        getQsort1D(T)(rows,data,Level_X_size);                        \
                                                                                \
        free(data);                                                             \
        data=NULL;                                                              \
        return 0;                                                               \
}
#define process_stream1D_IMPL(T) _process_stream1D_IMPL(T)

process_stream1D_IMPL(int);
process_stream1D_IMPL(float);
process_stream1D_IMPL(double);


////////////////////////////////////////////// Get qsort Exact Median 2D //////////////////////////////////////////////
#define _getQsort2D(T) getQsort2D$_##T##_$
#define getQsort2D(T) _getQsort2D(T)

#define _getQsort2D_IMPL(T)                                                                                           \
void getQsort2D(T)(int rows,int cols,int layers,T *data,T *Level_X,int Level_X_size)                                                  \
{                                                                                                                       \
        struct timeval start, end;                                                                                      \
        double median=0;                                                                                                \
        const size_t m = Level_X_size/2-1;                                                                                \
                                                                                                                        \
        gettimeofday(&start,NULL);                                                                                      \
        for(int k=0; k< layers; k++){                                                                                   \
                for(int i=0; i< rows; i++){                                                                             \
                        for(int j=0; j<cols; j++){                                                                      \
                                Level_X[i*cols+j] = data[k*rows*cols+i*cols+j];                                         \
                        }                                                                                               \
                }                                                                                                       \
                qsort(Level_X, Level_X_size, sizeof(T), cmpfunc(T));							\
		median=Level_X[m];                                                                                         \
        }                                                                                                               \
        gettimeofday(&end,NULL);                                                                                        \
        double endtime=(double)1000 * (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000.0);           \
        /*printf("qsort finding time is %fms\n",endtime);                                                        */       \
}
#define getQsort2D_IMPL(T) _getQsort2D_IMPL(T)

getQsort2D_IMPL(int);
getQsort2D_IMPL(float);
getQsort2D_IMPL(double);

////////////////////////////////  Process stream 2D  //////////////////////////////
#define _process_stream2D(T) process_stream2D$_##T##_$
#define process_stream2D(T) _process_stream2D(T)

#define _process_stream2D_IMPL(T)                                               \
int process_stream2D(T)(int rows, int cols,int layers, FILE *ori_fp)                    \
{                                                                               \
        int dataLength = rows*cols*layers;                                              \
        int Level_X_size= rows*cols;                                                 \
                                                                                \
        T *data =(T *)malloc(dataLength*sizeof(T));                             \
        fread(data,dataLength,sizeof(T),ori_fp);                                \
        fclose(ori_fp);                                                         \
        ori_fp = NULL;                                                          \
        T *Level_X =(T*)malloc(Level_X_size*sizeof(T));                         \
                                                                                \
        getQsort2D(T)(rows,cols,layers,data,Level_X,Level_X_size);                            \
                                                                                \
        free(data);                                                             \
        data=NULL;                                                              \
        free(Level_X);                                                          \
        Level_X=NULL;                                                           \
        return 0;                                                               \
}
#define process_stream2D_IMPL(T) _process_stream2D_IMPL(T)

process_stream2D_IMPL(int);
process_stream2D_IMPL(float);
process_stream2D_IMPL(double);


////////////////////////////////////////////// Get qsort Exact Median ///////////////////////////////////////////////
#define _getQsort3D(T) getQsort3D$_##T##_$
#define getQsort3D(T) _getQsort3D(T)

#define _getQsort3D_IMPL(T)                                                                                             \
void getQsort3D(T)(int rows, int cols, int layers,T *data, T *Level_X,int Level_X_size)                                 \
{                                                                                                                       \
        struct timeval start, end;                                                                                      \
        double median=0;                                                                                                \
        const size_t m = Level_X_size/2-1;                                                                                \
                                                                                                                        \
        gettimeofday(&start,NULL);                                                                                      \
        for(int k=0; k< layers; k++){                                                                                   \
                for(int i=0; i< rows; i++){                                                                             \
                        for(int j=0; j<cols; j++){                                                                      \
                                Level_X[j] = data[k*rows*cols+i*cols+j];                                                \
                        }                                                                                               \
                        qsort(Level_X, Level_X_size, sizeof(T), cmpfunc(T));                                            \
			median=Level_X[m];                                                                              \
                }                                                                                                       \
        }                                                                                                               \
        gettimeofday(&end,NULL);                                                                                        \
        double endtime=(double)1000 * (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000.0);           \
        /*printf("qsort finding time is %fms\n",endtime);                                                              */\
}
#define getQsort3D_IMPL(T) _getQsort3D_IMPL(T)

getQsort3D_IMPL(int);
getQsort3D_IMPL(float);
getQsort3D_IMPL(double);


////////////////////////////////  Process stream3D ///////////////////////////////
#define _process_stream3D(T) process_stream3D$_##T##_$
#define process_stream3D(T) _process_stream3D(T)

#define _process_stream3D_IMPL(T)                                                 \
int process_stream3D(T)(int rows, int cols, int layers, FILE *ori_fp)                   \
{                                                                               \
        int dataLength = rows*cols*layers;                                      \
        int Level_X_size= cols;                                                 \
                                                                                \
        T *data =(T *)malloc(dataLength*sizeof(T));                             \
        fread(data,dataLength,sizeof(T),ori_fp);                                \
        fclose(ori_fp);                                                         \
        ori_fp = NULL;                                                          \
        T *Level_X =(T*)malloc(Level_X_size*sizeof(T));                         \
                                                                                \
        getQsort3D(T)(rows,cols,layers,data,Level_X,Level_X_size);              \
                                                                                \
        free(data);                                                             \
        data=NULL;                                                              \
        free(Level_X);                                                          \
        Level_X=NULL;                                                           \
        return 0;                                                               \
}
#define process_stream3D_IMPL(T) _process_stream3D_IMPL(T)

process_stream3D_IMPL(int);
process_stream3D_IMPL(float);
process_stream3D_IMPL(double);


int main(int argc, char* argv[])
{
        char oriFilePath[640];
        char *dataType=NULL;
        int rows, cols, layers,dim;

        if(argc < 6){
                printf("Test case: Main [srcFilePath] [dimension sizes...] [data type] [dimebsion num]\n");
                printf("Example: Main data.dat 8 8 [128] I(int)|F(float)|D(double) 1|2|3\n");
                exit(0);
        }


        sprintf(oriFilePath,"%s", argv[1]);
        rows = atoi(argv[2]);
        cols = atoi(argv[3]);
        if(argc==6){ dataType = argv[4]; dim =atoi(argv[5]);}
        if(argc==7){ layers = atoi(argv[4]);dataType = argv[5]; dim =atoi(argv[6]);}

        FILE *ori_fp = fopen(oriFilePath,"r");
        if(ori_fp==NULL){
                printf("Cannot open file %s.\n",oriFilePath);
                return 0;
        }

        switch(*dataType){
                case 'I':
                        switch(dim){
                                case 1 :process_stream1D(int)(rows,cols,ori_fp);break;
                                case 2 :process_stream2D(int)(rows,cols,layers,ori_fp);break;
                        }
               case 'F':
                       switch(dim){
                               case 2 :process_stream2D(float)(rows,cols,layers,ori_fp);break;
                               case 3 :process_stream3D(float)(rows,cols,layers,ori_fp);break;
                       }
               case 'D':
                       switch(dim){
                               case 2 :process_stream2D(double)(rows,cols,layers,ori_fp);break;
                               case 3 :process_stream3D(double)(rows,cols,layers,ori_fp);break;
                       }
               default:break;
       }

       printf("comp_count is %d\n",cmp_count);
       printf("swap_count is %d\n",swap_count);

}
