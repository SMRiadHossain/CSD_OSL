#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>



#define MAX_KV_KEY_SIZE		16
#define __NR_csd_syscall		294




struct buff{
	char command[4];
};

struct csd_params {

	int ObjectID;
	char *data_pointer;
	struct buff buffer1;
} user_para;



int main(/*int argc, char **argv*/)
{
	user_para.buffer1.command[0]= 'a';
	user_para.buffer1.command[1]='0';
	user_para.ObjectID= 0;

	if(!syscall(__NR_csd_syscall,(void*) &user_para)) {
		printf("Systemcall successfull\n");
	} else {
		printf("Error systemcall\n");
	}


	return 0;
}
