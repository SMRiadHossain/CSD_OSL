#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_KV_KEY_SIZE		16
#define __NR_csd_syscall	294

struct buff{
	char command[4];
};

struct csd_params {

	int ObjectID;
	char* data_pointer;
	struct buff buffer1;
} user_para;



int main(int argc, char **argv)
{
	char w_data[4096] = "ABCDEabcde";
	char r_data[4096];

	user_para.buffer1.command[0]= 'c';
	user_para.data_pointer = w_data;
	
	if(!syscall(__NR_csd_syscall,(void*) &user_para)) {
		printf("Systemcall successfull\n");
	} else {
		printf("Error systemcall\n");
	}
	
	user_para.buffer1.command[0]= 'd';
	user_para.data_pointer = r_data;
	
	if(!syscall(__NR_csd_syscall,(void*) &user_para)) {
		printf("Systemcall successfull\n");
	} else {
		printf("Error systemcall\n");
	}

	printf("r_data: %s\n", r_data);
	
	user_para.buffer1.command[0]= 'e';
	
	if(!syscall(__NR_csd_syscall,(void*) &user_para)) {
		printf("Systemcall successfull\n");
	} else {
		printf("Error systemcall\n");
	}

	return 0;
}
