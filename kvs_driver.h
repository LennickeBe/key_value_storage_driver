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
#define KVS_SHOW_VAL _IOWR('k', 3, ioctl_arg *)
#define KVS_CLR_ARR _IO('k', 4)

#endif

// Device major
#define DEV_MAJOR 0
// Number of devices
#define DEV_COUNT 1
// entry length in chars
#define ENTRY_LEN 100
