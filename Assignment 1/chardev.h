/*
 *  chardev2.h - the header file with the ioctl definitions.
 *
 *  The declarations here have to be in a header file, because
 *  they need to be known both to the kernel module
 *  (in chardev.c) and the process calling ioctl (ioctl.c)
 */

#ifndef CHARDEV_H
#define CHARDEV_H

#include <linux/ioctl.h>

/*
 * The major device number. We can't rely on dynamic
 * registration any more, because ioctls need to know
 * it.
 */
#define MAJOR_NUM 100

#define IOCTL_SET_MSG _IOR(MAJOR_NUM, 0, char *)

#define IOCTL_GET_MSG _IOR(MAJOR_NUM, 1, char *)

#define IOCTL_GET_NTH_BYTE _IOWR(MAJOR_NUM, 2, int)

#define ACC_X_READ _IOR(MAJOR_NUM, 3, char *)
#define ACC_Y_READ _IOR(MAJOR_NUM, 4, char *)
#define ACC_Z_READ _IOR(MAJOR_NUM, 5, char *)

#define MAG_X_READ _IOR(MAJOR_NUM, 6, char *)
#define MAG_Y_READ _IOR(MAJOR_NUM, 7, char *)
#define MAG_Z_READ _IOR(MAJOR_NUM, 8, char *)

#define GYRO_X_READ _IOR(MAJOR_NUM, 9, char *)
#define GYRO_Y_READ _IOR(MAJOR_NUM, 10, char *)
#define GYRO_Z_READ _IOR(MAJOR_NUM, 11, char *)


#define DEVICE_FILE_NAME "/dev/ioctl_device"


#endif
