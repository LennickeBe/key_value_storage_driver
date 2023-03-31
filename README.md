# key_value_storage_driver

This project was done to show that I can familiarize myself with the subject of kernel module programming on my own.
The task was to create a small linux driver with stores key-value pairs using an xarray. Furthermore should the user-interaction be based on ioctl.

## Installation
After cloning first compile the source code:
```console
user@pc:~$ cd key_value_storage_driver
user@pc:~/key_value_storage_drive$ make
```
Load the module using insmod:
```console
user@pc:~/key_value_storage_driver$ sudo insmod kvs_driver.ko
```

In order to unload the module again you can use rmmod:
```console
user@pc:~$ sudo rmmod kvs_driver
```

## Usage
The kvs_app programm won't work without the module beeing loaded. (See [Installation](#installation))

Show the usage in command line:
```console
user@pc:~/key_storage_value_driver$ sudo ./kvs_app help
```
Add a value for the index "key". Key needs to be an integer and the length smaller than the ENTRY_LEN defined in kvs_driver.h (default is 100).
```console
user@pc:~/key_storage_value_driver$ sudo ./kvs_app add <key> <value>
```

Show the value for the index "key". Key needs to be an integer.
```console
user@pc:~/key_storage_value_driver$ sudo ./kvs_app show <key>
```

Remove the entry with index "key". Key needs to be an integer.
```console
user@pc:~/key_storage_value_driver$ sudo ./kvs_app rm <key>
```
  
Clear all key-value pairs.
```console
user@pc:~/key_storage_value_driver$ sudo ./kvs_app clear
```
