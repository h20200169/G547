/*
 *  chardev2.c - Create an input/output character device
 */
#include<linux/module.h>
#include<linux/version.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/cdev.h>
      /* We're doing kernel work */
      /* Specifically, a module */
#include <linux/fs.h>
#include <asm/uaccess.h>        /* for get_user and put_user */
#include <linux/time.h>
#include <linux/random.h>

#include "chardev.h"
#define SUCCESS 0
#define DEVICE_NAME "ioctl_device"
#define BUF_LEN 80
static dev_t first;

static struct cdev c_dev;
static struct class *cls;
static int Device_Open = 0;
static char Message;
static char *Message_Ptr;
static char msg_rec;


//struct timespec ts;

static int device_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "device opened\n");    
    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "DEVICE RELEASED\n");
    return SUCCESS;
}


static ssize_t device_read(struct file *file,   /* see include/linux/fs.h   */
                           char __user * buffer,        /* buffer to be
                                                         * filled with data */
                           size_t length,       /* length of the buffer     */
                           loff_t *offset)
{
	unsigned int i, reading;
	
	/*char reading[2];
        	reading[0]=52;
        	reading[1]=53;*/
        	get_random_bytes(&reading, 2);
        	reading &= 0x000003ff;
        	//a = (char*)reading;
        	
        	put_user(reading, buffer);
        	
        	/*for(i=0;i<length;i++)
        	{
        		put_user(reading[i], buffer+i);
        	}*/
	printk(KERN_INFO "THIS IS DEVICE_READ\n");
	
	return 0;

}
 


static ssize_t device_write(struct file *file,
             const char __user * buffer, size_t length, loff_t * offset)
{
       
    	get_user(msg_rec, buffer);
    
    
    printk(KERN_INFO "message received from user space, %d\n",msg_rec);
    return 0;
}


long device_ioctl(struct file *file,             /* ditto */
                  unsigned int ioctl_num,        /* number and param for ioctl */
                  unsigned long ioctl_param)
{
    int i;
    char *temp;
    char ch;
    char char_acc;
    temp=(char*)ioctl_param;

    switch (ioctl_num) 
    {
    	case IOCTL_SET_MSG:

         	device_write(file, temp, 1, 0);
         	break;

    	case IOCTL_GET_MSG:  
        	device_read(file, temp, 2, 0);
       	break;
	
    	case ACC_X_READ:
		device_read(file, temp, 2, 0);
              	break;
        
	case ACC_Y_READ:
		device_read(file, temp, 2, 0);
        	break;
        	
        case ACC_Z_READ:
		device_read(file, temp, 2, 0);
        	break;
        	
        case MAG_X_READ:
		device_read(file, temp, 2, 0);
        	break;
        	
        case MAG_Y_READ:
		device_read(file, temp, 2, 0);
        	break;
        	
        case MAG_Z_READ:
		device_read(file, temp, 2, 0);
        	break;
        	
        case GYRO_X_READ:
		device_read(file, temp, 2, 0);
        	break;
        
        case GYRO_Y_READ:
		device_read(file, temp, 2, 0);
        	break;
        	
        case GYRO_Z_READ:
		device_read(file, temp, 2, 0);
        	break;
    
    }
    return SUCCESS;
}

struct file_operations fops = {
        .read = device_read,
        .write = device_write,
        .unlocked_ioctl = device_ioctl,
        .open = device_open,
        .release = device_release,      /* a.k.a. close */
};

/*
 * Initialize the module - Register the character device
 */
int init_char_module(void)
{
	printk(KERN_INFO "namaste: chardev registered\n");
	
	if(alloc_chrdev_region(&first, 0, 1, "BITS_PILANI")<0)
	{
		return -1;
	}
	
	if((cls=class_create(THIS_MODULE, "char_dev"))==NULL)
	{	
		unregister_chrdev_region(first, 1);
		return -1;
	}
	
	if(device_create(cls, NULL, first, NULL, "ioctl_device")==NULL)
	{
		class_destroy(cls);
		unregister_chrdev_region(first, 1);
		return -1;
	}
	
	cdev_init(&c_dev, &fops);
	if(cdev_add(&c_dev, first, 1)==-1)
	{
		device_destroy(cls, first);
		class_destroy(cls);
		unregister_chrdev_region(first, 1);
		return -1;
	}
	return 0;
	
}
static void exit_module(void)
{
cdev_del(&c_dev);
device_destroy(cls,first);
class_destroy(cls);
unregister_chrdev_region(first,1);
printk(KERN_INFO "my char unregistered \n\n");
}


module_init(init_char_module);
module_exit(exit_module);


MODULE_LICENSE("GPL");
