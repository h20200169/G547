/*
 *  ioctl.c - the process to use ioctl's to control the kernel module
 *
 *  Until now we could have used cat for input and output.  But now
 *  we need to do ioctl's, which require writing our own process.
 */

/*
 * device specifics, such as ioctl numbers and the
 * major device file.
 */
#include "chardev.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>              /* open */
#include <unistd.h>             /* exit */
#include <sys/ioctl.h>          /* ioctl */
#include <string.h>

#define CONFIG_PARAM 4

/*
 * Functions for the ioctl calls
 */


int ioctl_set_msg(int file_desc, int *message)
{
    

    ioctl(file_desc, IOCTL_SET_MSG, message);

    return 0;
}

int acc_x_read(int file_desc)
{
    int ret_val;
    int *message;

    ioctl(file_desc, ACC_X_READ, message);

    printf("acc_x_read:%d\n", *message);
    return 0;
}

int acc_y_read(int file_desc)
{
    int ret_val;
    char message[3];

    ioctl(file_desc, ACC_Y_READ, message);
    printf("acc_y_read:%s\n", message);
    return 0;
}

int acc_z_read(int file_desc)
{
    int ret_val;
    char message[3];

    ioctl(file_desc, ACC_Z_READ, message);

    

    printf("acc_z_read:%s\n", message);
    return 0;
}

int mag_x_read(int file_desc)
{
    int ret_val;
    char message[3];

    ioctl(file_desc, MAG_X_READ, message);

    

    printf("mag_x_read:%s\n", message);
    return 0;
}

int mag_y_read(int file_desc)
{
    int ret_val;
    char message[3];

    ioctl(file_desc, MAG_Y_READ, message);

    printf("mag_y_read:%s\n", message);
    return 0;
}

int mag_z_read(int file_desc)
{
    int ret_val;
    char message[3];

    ioctl(file_desc, MAG_Z_READ, message);

    printf("mag_z_read:%s\n", message);
    return 0;
}

int gyro_x_read(int file_desc)
{
    int ret_val;
    char message[3];

    ioctl(file_desc, GYRO_X_READ, message);

    printf("gyro_x_read:%s\n", message);
    return 0;
}

int gyro_y_read(int file_desc)
{
    int ret_val;
    char message[3];

    ret_val = ioctl(file_desc, GYRO_Y_READ, message);

    printf("gyro_y_read:%s\n", message);
    return 0;
}

int gyro_z_read(int file_desc)
{
    int ret_val;
    char message[3];

    ret_val = ioctl(file_desc, GYRO_Z_READ, message);

    printf("gyro_z_read:%s\n", message);
    return 0;
}

int ioctl_get_nth_byte(int file_desc)
{
    //int i;
    int c;

    printf("get_nth_byte message:\n");

    //i = 0;
    //do {
        c = ioctl(file_desc, IOCTL_GET_NTH_BYTE,0);

        if (c < 0) {
            printf("ioctl_get_nth_byte failed\n");
            exit(-1);
        }

        printf("Second = %d \n\n",c);
    //} while (c != 0);
    putchar('\n');
    return 0;
}

int ioctl_get_acceleartor(int file_desc)
{
    int ret_val;
    char message[100];

  printf("testing successful\n");
    ret_val = ioctl(file_desc, IOCTL_GET_MSG, message);

    if (ret_val < 0) {
        printf("ioctl_get_msg failed:%d\n", ret_val);
        exit(-1);
    }
    printf("%c", message[0]);
    printf("get_msg message:%s\n", message);
    
    return 0;
}

/*
 * Main - Call the ioctl functions
 */
int main()
{
    int file_desc, ret_val, offset=48;//offset is used to get the symbol equivalent 
    int msg=64;
    //msg = CONFIG_PARAM + offset; //getting the ASCII code for 

    file_desc = open(DEVICE_FILE_NAME, 0);
    if (file_desc < 0) {
        printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
        exit(-1);
    }

   
	
	ioctl_set_msg(file_desc, &msg);
	acc_x_read(file_desc);
	
    close(file_desc);
    return 0;
}
