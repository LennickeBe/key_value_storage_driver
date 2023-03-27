#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "kvs_driver.h"

void _change_entry(int fd, int key, char *content)
{
	ioctl_arg io_arg;

	io_arg.key = key;
	io_arg.value = content;
	if (ioctl(fd, KVS_CHANGE_VAL, &io_arg) == -1)
	{
		perror("kvs_app ioctl change_entry");
	}
}


void _remove_entry(int fd, int key)
{
	ioctl_arg io_arg;

	io_arg.key = key;
	if (ioctl(fd, KVS_REMOVE_VAL, &io_arg) == -1)
	{
		perror("kvs_app ioctl remove_entry");
	}
}


void _show_entry(int fd, int key)
{
	ioctl_arg io_arg;
	io_arg.key = key;
	
	if (ioctl(fd, KVS_SHOW_VAL, &io_arg) == -1)
	{
		perror("kvs_app ioctl show_entry");
	}
	printf("Index:%i\tValue:%s\n", io_arg.key, io_arg.value);
}


void _clr_array(int fd)
{
	ioctl_arg io_arg;
	
	if (ioctl(fd, KVS_CLR_ARR) == -1)
	{
		perror("kvs_app ioctl clr_array");
	}
}


void _print_help(void)
{
 	// TODO
	printf("help");
}


int main(int argc, char *argv[])
{
	int fd, opt, key;
	char *value, *errstr;
	if ((fd = open("/dev/kvs", O_RDWR)))
	{
		perror("kvs_app open /dev/kvs");
	}
	if ((argc == 1) || (argc > 4))
	{
		_print_help();
	}

	close(fd);
}
