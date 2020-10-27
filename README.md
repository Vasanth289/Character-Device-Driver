# Character Device Driver

## Commmands used

$ make all

$ sudo insmod simple_char_device.ko

$ lsmod | grep simple

$ cat /proc/devices | grep simple

$ sudo mknod -m 666 /dev/simple_char c 240 0

$ ls -l /dev/simple_char

$ ./user_prog

$ cat /dev/simple_char

$ echo "Hello" > /dev/simple_char

$ sudo rmmod simple_char_device

$ sudo rm /dev/simple_char

$ make clean

## Makefile

obj-m := simple_char_device.o

all:

	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	
	$(CC) test.c -o test

clean:
	
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	
	rm test
