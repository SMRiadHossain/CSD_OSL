#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define GET_DISK_INFO 1
#define BLOCK_READ 2
#define BLOCKS_READ 3
#define BLOCK_WRITE 4
#define BLOCKS_WRITE 5

#define MAX_KV_KEY_SIZE		16
#define __NR_csd		555

struct buff{
	char command[4];
}buff1;

struct csd_params {

	int ObjectID;
	char *data_pointer;
	struct buff buff1;
} user_para;



int main(int argc, char **argv)
{
	buffer1.command[0]= 'a';
    buffer1.command[1]='b';
    user_para.ObjectID= 0;
    
    if(!syscall(__NR_csd, &params)) {
		printf("Systemcall successfull";
	} else {
		printf("Error systemcall");
	}
	

	return 0;
}
