obj-m := simple_char_device.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	$(CC) user_prog.c -o user_prog

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm user_prog
