#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdio.h>

#include "kvs_driver.h"

void _change_entry(int fd, int key, char *content)
{
	ioctl_arg io_arg;

	io_arg.key = key;
	strcpy(io_arg.value, content);
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
	
	if (ioctl(fd, KVS_SHOW_VAL, &io_arg) == -1)
	{
		perror("kvs_app ioctl remove_entry");
	}
	printf("%i\t%s\n", io_arg.key, io_arg.value);
}


int main(int argv, char *argc[])
{
	int fd;
	fd = open("/dev/kvs", O_RDWR);
	_change_entry(fd, 1, "testy");
	_show_entry(fd, 1);
	_change_entry(fd, 1, "whack");
	_show_entry(fd, 1);
	_remove_entry(fd, 1);
	_show_entry(fd, 1);

	close(fd);
}
