//geneNormal.c
//Author: Fenfang Li
//Time:4/2023


/************* Using ziggurat method to generate normal distribution data ************/

# include <stdlib.h>
# include <stdio.h>
# include <stdint.h>

# include "ziggurat_inline.h"

int main();
void z_nor (int cols);
void z_nor_time(int sample_num);

float *temp; 

int main(int argc, char *argv[]){

	int sample_num = 1000000;
	int rows, cols,layers;
	int dataLength;
	char outFileName[300];
	timestamp ( );
	printf ( "\n" );
	printf ( "/********************************* ZIGGURAT_NORMAL_DISTRIBUTION_DATA_GENERATING *****************************/\n" );
	printf ( "  C version:\n" );
	
	if(argc <2){
		printf("Test case:geneNormal [dimension sizes...] [output file name]\n");
		printf("Example:geneNormal datasize normal\n");
		exit(0);
	}

	cols = atoi(argv[1]);
	sprintf(outFileName,"%s.bin", argv[2]);

	temp = (float*)calloc(cols,sizeof(float));

	/*Generate the normal distribution data*/
	printf ( "\n" );
	printf ( "Z_NOR\n" );
	printf ( "  R4_NOR_VALUE is an inline generator of pseudorandom normally\n" );
	printf ( "  distributed floats.\n" );
	printf ( "\n" );

	z_nor (cols);
	printf ( "\n" );
	int i=0;
	
	FILE *fp;
	fp = fopen(outFileName,"wb");
	fwrite(temp,sizeof(float),cols,fp);
	
	/* Terminate.*/
	printf ( "\n" );
   	printf ( "/********************************* ZIGGURAT_NORMAL_DISTRIBUTION_DATA_GENERATING *****************************/\n" );
   	printf ( "  Normal end of execution.\n" );
   	printf ( "\n" );
   	timestamp ( );

	free(temp);
	temp=NULL;
   	return 0;
}

void z_nor(int cols){
	int i,j;
	uint32_t jcong_value;
	uint32_t jsr_value;
	float value;
	float sumValue=0;
	uint32_t w_value;
	uint32_t z_value;


	jsr_value = 987654321;
	jcong_value = 198765432;
	w_value = 219876543;
	z_value = 321987654;

	/* Call ZIGSET to set the seed.*/

	  zigset ( jsr_value, jcong_value, w_value, z_value );

	  for ( i = 0; i < cols; i++ )
	  {
	      value = r4_nor_value( );
	      temp[i]=value;
	      sumValue+=value;
	      //printf ( "  %6d  %14f\n", i, value );
	  }
	  printf(" the mean of normal data is: %14f\n",sumValue/10000);
	
	return;
}

void z_nor_test( ){
	int i,j;
	uint32_t jcong_value;
	uint32_t jsr_value;
	float value;
	float sumValue=0;
	uint32_t w_value;
	uint32_t z_value;

	printf ( "\n" );
	printf ( "Z_NOR_TEST\n" );
	printf ( "  R4_NOR_VALUE is an inline generator of pseudorandom normally\n" );
	printf ( "  distributed floats.\n" );

	for ( j = 0; j < 3; j++ )
	{
	  if ( ( j % 2 ) == 0 )
	  {
	    jsr_value = 123456789;
	    jcong_value = 234567891;
	    w_value = 345678912;
	    z_value = 456789123;
	  }
	  else
	  {
	    jsr_value = 987654321;
	    jcong_value = 198765432;
	    w_value = 219876543;
	    z_value = 321987654;
	  }

	  printf ( "\n" );
	  printf ( "  Call ZIGSET with these seeds:\n" );
	  printf ( "\n" );
	  printf ( "  jsr_value   = %u\n", jsr_value );
	  printf ( "  jcong_value = %u\n", jcong_value );
	  printf ( "  w_value     = %u\n", w_value );
	  printf ( "  z_value     = %u\n", z_value );

	/* Call ZIGSET to set the seed.*/

	  zigset ( jsr_value, jcong_value, w_value, z_value );

	  for ( i = 1; i <= 500; i++ )
	  {
	      value = r4_nor_value ( );
	      sumValue+=value;
	  }
	  printf(" the mean of normal data is: %14f\n",sumValue/500);
	  sumValue=0;
	}

	return;
}

void z_nor_time(int sample_num){
	  double ctime;
	  uint32_t jcong_value;
	  uint32_t jsr_value;
	  int sample;
	  float value;
	  uint32_t w_value;
	  uint32_t z_value;

	  printf ( "\n" );
	  printf ( "Z_NOR_TIME\n" );
	  printf ( "  Measure the time it takes R4_NOR_VALUE to generate\n" );
	  printf ( "  %d normal random float values.\n", sample_num );

	  /* Call ZIGSET to set the seed.*/
	  jsr_value = 123456789;
	  jcong_value = 234567891;
	  w_value = 345678912;
	  z_value = 456789123;
	  zigset ( jsr_value, jcong_value, w_value, z_value );

	  ctime = cpu_time ( );

	  for ( sample = 0; sample < sample_num; sample++ )
	  {
	    value = r4_nor_value ( );
	  }
	  ctime = cpu_time ( ) - ctime;

	  printf ( "\n" );
	  printf ( "  %f seconds.\n", ctime );

	  return;
}

