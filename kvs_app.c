#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

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
	printf("test\n");
	printf("%i\n", io_arg.key);
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
	char *value;

	if ((fd = open("/dev/kvs", O_RDWR)) == -1)
	{
		perror("kvs_app open /dev/kvs");
		return -1;
	}

	if (1)
	{
		_change_entry(fd, 1, "test");
		_show_entry(fd, 1);
		_remove_entry(fd, 1);
		_show_entry(fd, 1);
		_change_entry(fd, 1, "test");
		_change_entry(fd, 2, "foo");
		_show_entry(fd, 1);
		_show_entry(fd, 2);
		_clr_array(fd);
		_show_entry(fd, 1);
		_show_entry(fd, 2);
		close(fd);
		return 0;
	}


	if ((argc < 2) || (argc > 4))
	{
		_print_help();
		close(fd);
		return -1;
	}


	if (strcmp(argv[1], "add") == 0)
	{
		key = strtol(argv[2], NULL, 0);
		if (errno!=0)
		{
			perror("kvs_app parse index");
			_print_help();
			return -1;
		}
		value = argv[3];
		_change_entry(fd, key, value);
		close(fd);
		return 0;
	}
	
	if (strcmp(argv[1], "show") == 0)
	{
		key = strtol(argv[2], NULL, 0);
		_show_entry(fd, key);
		close(fd);
		return 0;
	}

	if (strcmp(argv[1], "rm") == 0)
	{
		key = strtol(argv[2], NULL, 0);
		_remove_entry(fd, key);
		close(fd);
		return 0;
	}

	if (strcmp(argv[1], "clear") == 0)
	{
		_clr_array(fd);
		close(fd);
		return 0;
	}

	close(fd);
}
