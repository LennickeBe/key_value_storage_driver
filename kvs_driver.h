#ifndef KVS_IOCTL_H
#define KVS_IOCTL_H
#include <linux/ioctl.h>

typedef struct
{
	int key;
	char value[100];
} ioctl_arg;

#define KVS_CHANGE_VAL _IOW('k', 1, ioctl_arg *)
#define KVS_REMOVE_VAL _IOW('k', 2, ioctl_arg *)
#define KVS_SHOW_VAL _IOR('k', 3, ioctl_arg *)
#define KVS_SHOW_ARR _IOR('k', 4)
#define KVS_CLR_ARR _IO('k', 5)

#endif

#ifndef DEVICE_NUM
#define DEV_MAJOR 0
#define DEV_COUNT 1
#endif

