#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "kvs_driver.h"

static void change_entry(int fd, int key, char *content)
{
	ioctl_arg io_arg;

	io_arg.key = key;
	strncpy(io_arg.value, content, ENTRY_LEN * sizeof(char));
	if (ioctl(fd, KVS_CHANGE_VAL, &io_arg) == -1)
	{
		perror("kvs_app ioctl change_entry");
	}
}


static void remove_entry(int fd, int key)
{
	ioctl_arg io_arg;

	io_arg.key = key;
	if (ioctl(fd, KVS_REMOVE_VAL, &io_arg) == -1)
	{
		perror("kvs_app ioctl remove_entry");
	}
}


static void show_entry(int fd, int key)
{
	ioctl_arg io_arg;
	io_arg.key = key;
	
	if (ioctl(fd, KVS_SHOW_VAL, &io_arg) == -1)
	{
		perror("kvs_app ioctl show_entry");
	}
	printf("%i:\t%s\n", io_arg.key, io_arg.value);
}


static void clr_array(int fd)
{
	ioctl_arg io_arg;
	
	if (ioctl(fd, KVS_CLR_ARR) == -1)
	{
		perror("kvs_app ioctl clr_array");
	}
}


static void print_help(void)
{
	printf("Usage: sudo ./kvs_app [command] [options]\n\n");
	printf("Change value (key must be an integer, value %i or less chars)\n\n", ENTRY_LEN);
	printf("\t sudo ./kvs_app add <key> <value>\n\n");
	printf("Show value (key must be an integer)\n\n");
	printf("\t sudo ./kvs_app show <key>\n\n");
	printf("Remove value (key must be an integer)\n\n");
	printf("\t sudo ./kvs_app rm <key>\n\n");
	printf("Clear entire xarray\n\n");
	printf("\t sudo ./kvs_app clear\n\n");
}


int main(int argc, char *argv[])
{
	int fd, opt, key;
	char value[ENTRY_LEN];

	if ((fd = open("/dev/kvs", O_RDWR)) == -1)
	{
		perror("kvs_app open /dev/kvs");
		return -1;
	}


	if ((argc < 2) || (argc > 4))
	{
		print_help();
		close(fd);
		return -1;
	}

	if (strcmp(argv[1], "help") == 0)
	{
		print_help();
		return 0;
	}

	if (strcmp(argv[1], "add") == 0)
	{
		key = strtol(argv[2], NULL, 0);
		if (errno!=0)
		{
			perror("kvs_app parse index");
			print_help();
			return -1;
		}
		if (strlen(argv[3]) > ENTRY_LEN)
		{
			perror("kvs_app add value-too-long");
			print_help();
			return -1;
		}
		strncpy(value, argv[3], ENTRY_LEN * sizeof(char));
		change_entry(fd, key, value);
		close(fd);
		return 0;
	}
	
	if (strcmp(argv[1], "show") == 0)
	{
		key = strtol(argv[2], NULL, 0);
		show_entry(fd, key);
		close(fd);
		return 0;
	}

	if (strcmp(argv[1], "rm") == 0)
	{
		key = strtol(argv[2], NULL, 0);
		remove_entry(fd, key);
		close(fd);
		return 0;
	}

	if (strcmp(argv[1], "clear") == 0)
	{
		clr_array(fd);
		close(fd);
		return 0;
	}

	close(fd);
	printf("Could not match given arguments to commands.\nUse \"sudo ./kvs_app help\" to show usage.\n");
	return 0;
}
