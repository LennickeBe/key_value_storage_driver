#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#include "kvs_driver.h"

void _change_entry(int fd)
{
	ioctl_arg io_arg;

	io_arg.key = 1;
	strcpy(io_arg.value, "testy");
	ioctl(fd, KVS_CHANGE_VAL, &io_arg);
}


int main(int argv, char *argc[])
{
	int fd;
	fd = open("/dev/kvs", O_RDWR);
	_change_entry(fd);
	close(fd);
}
