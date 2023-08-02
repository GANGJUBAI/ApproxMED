//CAS_Main_qSelect.c
//Author:Fenfang Li
//Time:2023/3

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include "CAS_Main_qSelect.h"

/* record the number of comparsion and swap */
int cmp_count = 0;
int swap_count = 0;

int main(int argc, char* argv[])
{
	char oriFilePath[640];
	char *dataType=NULL;
	int rows, cols,layers,dim,loops,loopDim=0;
	srand((int)time(NULL));

	if(argc < 6){
		printf("Test case: Main [srcFilePath] [dimension sizes...] [data type] [dimension num]\n");
		printf("Example: Main data.dat 8 8 [128] I(int)|F(float)|D(double) 1|2|3 10 2(2D loop)|3(3D loop)\n");
		exit(0);
	}


	sprintf(oriFilePath,"%s", argv[1]);
	rows = atoi(argv[2]);
	cols = atoi(argv[3]);
	if(argc==6){ dataType = argv[4]; dim =atoi(argv[5]);}
	if(argc==7){ layers = atoi(argv[4]); dataType = argv[5]; dim =atoi(argv[6]);}
    if(argc==9){ layers = atoi(argv[4]); dataType = argv[5]; dim =atoi(argv[6]); loops = atoi(argv[7]); loopDim=atoi(argv[8]);}
		

	FILE *ori_fp = fopen(oriFilePath,"r");
	if(ori_fp==NULL){
		printf("Cannot open file %s.\n",oriFilePath);
		return 0;
	}

	switch(*dataType){ 
		case 'I': 
			switch(dim){ 
				case 1 : process_stream1D(int)(rows,cols,ori_fp); break;
				case 2 :if(loopDim =2)  {process_stream2Ds3(int)(rows,cols,layers,loops,ori_fp);break;}
                         process_stream2D(int)(rows,cols,layers,ori_fp); break;
				case 3 :if(loopDim ==3)  {process_stream3D3(int)(rows,cols,layers,loops,ori_fp);break;}
                         process_stream3D2(int)(rows,cols,layers,ori_fp);break;
			} 
		case 'F': 
			switch(dim){ 
                case 1 : process_stream1D(int)(rows,cols,ori_fp);break;
				case 2 : process_stream2D3(float)(rows,cols,layers,ori_fp); break;
				case 3 : process_stream3D(float)(rows,cols,layers,ori_fp); break;
			} 
		case 'D': 
			switch(dim){ 
				case 2 : process_stream2D3(double)(rows,cols,layers,ori_fp); break;
				case 3 : process_stream3D(double)(rows,cols,layers,ori_fp); break;
			} 
		default:break; 
	} 
    
    printf("comp_count is %d\n", cmp_count);
    printf("swap_count is %d\n", swap_count);
}
