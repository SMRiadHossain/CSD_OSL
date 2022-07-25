/* this one is based on syscall and struct parameter passing as you first told to implement syscall for device interface with struct. */

#include<linux/kernel.h>
#include<linux/syscalls.h>
#include<linux/uaccess.h>

#define INITIALIZE_BUCKET	'a'
#define GET_BUCKET_INFO		'b'
#define PUT_OBJECT		'c'
#define GET_OBJECT		'd'
#define DEL_OBJECT		'e'

struct buff{
	char command[4];
};

struct csd_params {

	int ObjectID;
	char *data_pointer;
	struct buff buff_params;
};

//osl_externalinterface_functions.
void osl_externalinterface_initializebucket(struct csd_params *k_para)
{
	printk("osl_externalinterface_initializebucket function executed\n");
	return;
}

void osl_externalinterface_getbucketinfo(struct csd_params *k_para)
{

	printk("osl_externalinterface_getbucketinfo function executed\n");
	return;
}

void osl_externalinterface_putobject(struct csd_params *k_para)
{
	printk("osl_externalinterface_putobject function executed\n");
	return;
}

void osl_externalinterface_getobject(struct csd_params *k_para)
{
	printk("osl_externalinterface_getobject function executed\n");
	return;
}

void osl_externalinterface_delobject(struct csd_params *k_para)
{
	printk("osl_externalinterface_delobject function execute\n");
	return;
}




//336
SYSCALL_DEFINE1(csd_syscall, const void*, user_para) {

	struct csd_params k_para;

	copy_from_user(&k_para,(struct csd_params*) user_para,sizeof(struct csd_params));//copy from user function uaccess.h

	//struct buff buff1;
    printk("I'm here\n");

	char CommandID = k_para.buff_params.command[0];
	char BucketID = k_para.buff_params.command[1];

    printk("%c %c\n", CommandID, BucketID);

	/*if(!strncpy_from_user(k_bucketID, u_bucketID, 255)) {//have to improve
	  printk("Error resolving parameters\n");
	  return -1;  
	  }*/

	/*if(copy_from_user(&data, DPTR, c_ID)
	  printk("Bucket-> %s created.\n", k_bucketID);*/

	switch(CommandID)
	{
		case INITIALIZE_BUCKET:
			osl_externalinterface_initializebucket(&k_para);
			break;
		case GET_BUCKET_INFO:
			osl_externalinterface_getbucketinfo(&k_para);
			break;
		case PUT_OBJECT:
			osl_externalinterface_putobject(&k_para);
			break;
		case GET_OBJECT:
			osl_externalinterface_getobject(&k_para);
			break;
		case DEL_OBJECT:
			osl_externalinterface_delobject(&k_para);
			break;
		default:
			printk("default function executed");
	}
	return 0;
}

MODULE_LICENSE("GPL");
