#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>



/*------------------------------
	Data File Directory
------------------------------*/
#define FILE_DIR "cifar-10-batches-bin"
#define FILE_DIR_BUF_LEN (256)
#define OUTPUT_DIR "output"


const float YUV[9]={0.299,0.587,0.114,-0.1687,-0.3313,0.5,0.5,-0.4187,-0.0813};
const float YUV_inv[9]={1.0,0.0,1.4012,1.0,-0.34414,-0.71414,1.0,1.772,0.0};

int read_file_and_convert(char *data_filename,char *output_filename){

	int data = 0;
	int output = 0;
	char buf[3073];
	float YUV_pixel[3];
	float YUV_revised[3];
	int idx = 0;
  int idx2 = 0;

	//[kira]:Open Data Bin File
	data = open(data_filename,O_RDONLY);
	if(data<0){
		goto OPEN_DATA_FAIL;
	}
	//[kira]:open output file
	output = open(output_filename,O_RDWR|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	if(output<0){
		goto OPEN_OUTPUT_FAIL;
	}
	
	//[kira]:read data file
	while(read(data,buf,3073)==3073){
    //[kira]:RGB to YUV
		for(idx=1;idx<=1024;idx++){
			YUV_pixel[0] = buf[idx]*YUV[0]+buf[idx+1024]*YUV[1]+buf[idx+2048]*YUV[2];
			YUV_pixel[1] = buf[idx]*YUV[3]+buf[idx+1024]*YUV[4]+buf[idx+2048]*YUV[5]+128;
			YUV_pixel[2] = buf[idx]*YUV[6]+buf[idx+1024]*YUV[7]+buf[idx+2048]*YUV[8]+128;
			buf[idx] = (char) YUV_pixel[0];
			buf[idx+1024] = (char) YUV_pixel[1];
			buf[idx+2048] = (char) YUV_pixel[2];
		}

    //down sampling
    for(idx=0;idx<16;idx++){
      for(idx2=0;idx2<16;idx2++){
        int start = idx*64+idx2*2+1;
        int lefttop = start+1, righttop = start+2, bottomleft = start+33, bottomright = start+34;
        int sample_idx = 0;
        int temp_U, temp_V;
        temp_U = (buf[lefttop+1024]+buf[righttop+1024]+ \
                 buf[bottomleft+1024]+buf[bottomright+1024])/4;
        temp_V = (buf[lefttop+2048]+buf[righttop+2048]+ \
                 buf[bottomleft+2048]+buf[bottomright+2048])/4;
        buf[lefttop+1024]=temp_U;
        buf[lefttop+2048]=temp_V;
        buf[righttop+1024]=temp_U;
        buf[righttop+2048]=temp_V;
        buf[bottomleft+1024]=temp_U;
        buf[bottomleft+2048]=temp_V;
        buf[bottomright+1024]=temp_U;
        buf[bottomright+2048]=temp_V;
      }
    }

    //[kira]:revise YUV
    for(idx=1;idx<=1024;idx++){
      YUV_pixel[0] = buf[idx];
      YUV_pixel[1] = buf[idx]+buf[idx+1024]-128;
      YUV_pixel[2] = buf[idx]+buf[idx+2048]-128;
      buf[idx] = (char) YUV_pixel[0];
      buf[idx+1024] = (char) YUV_pixel[1];
      buf[idx+2048] = (char) YUV_pixel[2];

    }

    //[kira]:back to RGB
/*		for(idx=1;idx<=1024;idx++){
			YUV_pixel[0] = buf[idx]*YUV_inv[0]+buf[idx+1024]*YUV_inv[1]+(buf[idx+2048]-128)*YUV_inv[2];
			YUV_pixel[1] = buf[idx]*YUV_inv[3]+(buf[idx+1024]-128)*YUV_inv[4]+(buf[idx+2048]-128)*YUV_inv[5];
			YUV_pixel[2] = buf[idx]*YUV_inv[6]+(buf[idx+1024]-128)*YUV_inv[7]+buf[idx+2048]*YUV_inv[8];
			buf[idx] = (char) YUV_pixel[0];
			buf[idx+1024] = (char) YUV_pixel[1];
			buf[idx+2048] = (char) YUV_pixel[2];
		}*/

		write(output,buf,3073);	
	}
	

	close(output);
	return 0;
OPEN_OUTPUT_FAIL:
	close(data);
OPEN_DATA_FAIL:
	return -1;
}

void set_file_name(char *buf,char *dirname,char *filename){
	strcpy(buf,dirname);
	strcat(buf,"/");
	strcat(buf,filename);
	return;
}

int main(int argc, char **argv){

	int i = 0;
	char file_dir[FILE_DIR_BUF_LEN];
	char output_dir[FILE_DIR_BUF_LEN];
	char data[5][256];
	char output[5][256];
  char test_input[256];
  char test_output[256];
  char buf[256];

    //[kira]:make sure input file
	getcwd(file_dir,FILE_DIR_BUF_LEN);
	strcat(file_dir,"/");
	strcat(file_dir,FILE_DIR);
	for(i=0;i<5;i++){
		char name[256];
		sprintf(name,"data_batch_%d.bin",i+1);
		set_file_name(data[i],file_dir,name);
	}
  sprintf(buf,"test_batch.bin");
  set_file_name(test_input,file_dir,buf);

	//[kira]:make sure output file
	getcwd(output_dir,FILE_DIR_BUF_LEN);
	strcat(output_dir,"/");
	strcat(output_dir,OUTPUT_DIR);
	strcat(output_dir,"/");
	strcat(output_dir,FILE_DIR);
	for(i=0;i<5;i++){
		char name[256];
		sprintf(name,"data_batch_%d.bin",i+1);
		set_file_name(output[i],output_dir,name);
	}

  //[kira]:convert test file
  sprintf(buf,"test_batch.bin");
  set_file_name(test_output,output_dir,buf);

	for(i=0;i<5;i++){
		if(read_file_and_convert(data[i],output[i])<0){
			printf("Cannot convert %s\n",data[i]);
			return -1;
		}
	}

  read_file_and_convert(test_input,test_output);

	return 0;
}
