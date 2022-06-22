/* this one is based on syscall and struct parameter passing as you first told to implement syscall for device interface with struct. */

#include<linux/kernel.h>
#include<linux/syscalls.h>

#define INITIALIZEBUCKET	1
#define GETBUCKETINFO		2
#define PUTOBJECT		3
#define GETOBJECT		4
#define DELOBJECT		5




//osl_externalinterface_functions.
void osl_externalinterface_initializebucket(void)
{
	printk("osl_externalinterface_initializebucket function executed");
	return;
}

void osl_externalinterface_getbucketinfo(void)
{
	printk("osl_externalinterface_getbucketinfo function executed");
	return;
}

void osl_externalinterface_putobject(void)
{
	printk("osl_externalinterface_putobject function executed");
	return;
}

void osl_externalinterface_getobject(void)
{
	printk("osl_externalinterface_getobject function executed");
	return;
}

void osl_externalinterface_delobject(void)
{
	printk("osl_externalinterface_delobject function executed");
	return;
}





//335
SYSCALL_DEFINE1(csd_syscall, struct user_para) {

	

	if(!strncpy_from_user(k_bucketID, u_bucketID, 255)) {
		printk("Error resolving parameters\n");
		return -1;  
	}
	
	/*if(copy_from_user(&data, DPTR, c_ID)
	printk("Bucket-> %s created.\n", k_bucketID);*/
	
	char commandID = usr_para.command[0];
	
	switch(commandID)
	{
		case INITIALIZE_BUCKET:
		    osl_externalinterface_initializebucket();
            break;
		case GET_BUCKET_INFO:
		    osl_externalinterface_getbucketinfo();
            break;
		case :PUT_OBJECT
		    osl_externalinterface_putobject();
            break;
		case :GET_OBJECT
		    osl_externalinterface_getobject();
            break;
		case DEL_OBJECT:
		    osl_externalinterface_delobject();
            break;
		default:
		printk("default function executed");
	}
	return 0;
}

MODULE_LICENSE("GPL");
