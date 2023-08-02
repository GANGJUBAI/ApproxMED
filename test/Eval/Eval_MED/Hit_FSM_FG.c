//Main_FSM_ori.c
//Author:Fenfang Li
//Time:2023/5

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "Hit_FSM_FG.h"


int main(int argc, char* argv[])
{

	char oriFilePath[640];
	char *dataType=NULL;
	int rows, cols, layers,dim,loops,loopDim=0;
    int groupNum;

	if(argc < 6){
		printf("Test case: Main [srcFilePath] [dimension sizes...] [data type] [dimension num]\n");
		printf("Example: Main data.dat 8 8 [128] I(int)|F(float)|D(double) 1|2|3 10 2(2D loop)|3(3D loop)\n");
		exit(0);
	}


	sprintf(oriFilePath,"%s", argv[1]);
	rows = atoi(argv[2]);
	cols = atoi(argv[3]);
	if(argc==7){ dataType = argv[4]; dim =atoi(argv[5]);groupNum=atoi(argv[6]);}
	if(argc==8){ layers = atoi(argv[4]); dataType = argv[5]; dim =atoi(argv[6]);groupNum=atoi(argv[7]);}
    if(argc==10){ layers = atoi(argv[4]); dataType = argv[5]; dim =atoi(argv[6]); loops = atoi(argv[7]); loopDim=atoi(argv[8]);groupNum=atoi(argv[9]);}
	
	FILE *ori_fp = fopen(oriFilePath,"r");
	if(ori_fp==NULL){
		printf("Cannot open file %s.\n",oriFilePath);
	}
	switch(*dataType){
		case 'I':
			switch(dim){
				case 1 :return process_stream1D(int)(rows,cols,ori_fp,groupNum);
				case 2 :if(loopDim ==2) return process_stream2Ds3(int)(rows,cols,layers,loops,ori_fp,groupNum);
                        return process_stream2D(int)(rows,cols,layers,ori_fp,groupNum);
				case 3 :if(loopDim ==3) return process_stream3D3(int)(rows,cols,layers,loops,ori_fp,groupNum);
                        return process_stream3D2(int)(rows,cols,layers,ori_fp,groupNum);
			}
		case 'F':
			switch(dim){
                case 1 :return process_stream1D(int)(rows,cols,ori_fp,groupNum);
				case 2 :return process_stream2D3(float)(rows,cols,layers,ori_fp,groupNum);
				case 3 :return process_stream3D(float)(rows,cols,layers,ori_fp,groupNum);
			}
		case 'D':
			switch(dim){
				case 2 :return process_stream2D3(double)(rows,cols,layers,ori_fp,groupNum);
				case 3 :return process_stream3D(double)(rows,cols,layers,ori_fp,groupNum);
			}
		default:break;
	}
}
