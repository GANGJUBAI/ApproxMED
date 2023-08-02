//geneConstant.c
//Author: Fenfang Li
//Time:4/2023

# include <stdlib.h>
# include <stdio.h>
# include <stdint.h>

int main(int argc, char *argv[]){
    int dataSize;
    float element;
    char outFileName[300];

    printf ( "\n" );
	printf ( "/********************************* CONSTANT_DATA_GENERATING *****************************/\n" );
    printf ( "  C version:\n" );

    if(argc <3){
		printf("Test case:geneConstant [data sizes...] [element number][output file name]\n");
		printf("Example:geneConstant datasize 1.0 constant\n");
		exit(0);
	}

    dataSize = atoi(argv[1]);
    element = atof(argv[2]);
	sprintf(outFileName,"%s.bin", argv[3]);
    

    float* data =(float*)calloc(dataSize,sizeof(float));
    for(int i=0;i<dataSize;i++){
        data[i]= element;
    }
    
    printf("The data generated will be saved into file :%s\n",outFileName);
    FILE *fp = fopen(outFileName,"wb");
	fwrite(data,sizeof(float),dataSize,fp);

    printf ( "\n" );
    printf ( "/********************************* CONSTANT_DATA_GENERATING *****************************/\n" );
    printf ( "  Constant end of execution.\n" );
   	printf ( "\n" );

    free(data);
    data=NULL;
    return 0;

}
