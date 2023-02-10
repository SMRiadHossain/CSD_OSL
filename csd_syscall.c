#include<linux/kernel.h>
#include<linux/syscalls.h>
#include<linux/uaccess.h>
#include <linux/nvme_ioctl.h>
#include <linux/nvme.h>
#include <linux/blkdev.h>

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
	char __user *data_pointer;
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
	
	struct nvme_user_io uio;
	struct file *fd = filp_open("/dev/nvme0n1", O_WRONLY | O_DIRECT, 0);
	
	struct inode* inode = (fd->f_mapping->host);
	struct block_device* bdev = I_BDEV(inode);
	struct gendisk* bd_disk = (struct gendisk*)bdev->bd_disk;
	
	//Here we will populate nvme_user_io

	uio.opcode = nvme_cmd_write;
	uio.flags = 0;
	uio.control = 0;
	uio.metadata = (unsigned long) 0;
	uio.addr = (unsigned long) k_para->data_pointer; // data;
	uio.slba = 0;
	uio.nblocks = 1;struct file *fd = filp_open("/dev/nvme0n1", O_WRONLY, 0);
	
	struct inode* inode = (fd->f_mapping->host);
	struct block_device* bdev = I_BDEV(inode);
	uio.dsmgmt = 0;
	uio.reftag = 0;
	uio.apptag = 0;
	uio.appmask = 0;

	//mm_segment_t old_fs = get_fs();
	//set_fs(KERNEL_DS);
	mm_segment_t old_fs;
	ssize_t result;

	old_fs = get_fs();
	set_fs(get_ds());


	// fd->f_op->unlocked_ioctl(fd, NVME_IOCTL_SUBMIT_IO, (unsigned long) &uio);
	bd_disk->fops->ioctl(bdev, 0, NVME_IOCTL_SUBMIT_IO, (unsigned long) &uio);
	set_fs(old_fs);

	filp_close(fd, 0);

	return;
}

void osl_externalinterface_getobject(struct csd_params *k_para)
{
	printk("osl_externalinterface_getobject function executed\n");
	
	struct nvme_user_io uio;
	struct file *fd = filp_open("/dev/nvme0n1", O_WRONLY, 0);
	
	struct inode* inode = (fd->f_mapping->host);
	struct block_device* bdev = I_BDEV(inode);
	struct gendisk* bd_disk = (struct gendisk*)bdev->bd_disk;
	
	//Here we will populate nvme_user_io

	uio.opcode = nvme_cmd_read;
	uio.flags = 0;
	uio.control = 0;
	uio.metadata = (unsigned long) 0;
	uio.addr = (void __user *) k_para->data_pointer; // data;
	uio.slba = 0;
	uio.nblocks = 1;
	uio.dsmgmt = 0;
	uio.reftag = 0;
	uio.apptag = 0;
	uio.appmask = 0;

	//mm_segment_t old_fs = get_fs();
	//set_fs(KERNEL_DS);
	mm_segment_t old_fs;
	ssize_t result;

	old_fs = get_fs();
	set_fs(get_ds());


	// fd->f_op->unlocked_ioctl(fd, NVME_IOCTL_SUBMIT_IO, (unsigned long) &uio);
	bd_disk->fops->ioctl(bdev, 0, NVME_IOCTL_SUBMIT_IO, (void __user *) &uio);
	set_fs(old_fs);

	filp_close(fd, 0);

	return;
}

void osl_externalinterface_delobject(struct csd_params *k_para)
{
	printk("osl_externalinterface_delobject function execute\n");

	struct file *fd = filp_open("/dev/nvme0n1", O_WRONLY, 0);
	struct inode* inode = (fd->f_mapping->host);
	struct block_device* bdev = I_BDEV(inode);

	blkdev_ioctl(bdev, 0, BLKDISCARD, k_para->data_pointer, 4096);

	return;
}


SYSCALL_DEFINE1(csd_syscall, const void __user *, user_para) {

	struct csd_params k_para;

	copy_from_user(&k_para,(struct csd_params*) user_para,sizeof(struct csd_params));//copy from user function uaccess.h

	//struct buff buff1;

	char CommandID = k_para.buff_params.command[0];

	printk("%c\n", CommandID);

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