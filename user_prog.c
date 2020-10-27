#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BUFFER_LENGTH 1024

int main ()
{
	char buffer[BUFFER_LENGTH];
	int temp, fd;
	fd=open ("/dev/simple_char",O_RDWR);
	if(fd < 0)
	{
		perror("Failed to open the device");
		return errno;
	}
	printf("Enter the message : ");
	scanf("%[^\n]%*c",buffer);
	temp = write(fd,buffer,BUFFER_LENGTH);
	printf("Writing message to the device [%s].\n", buffer);
	if(temp < 0)
	{
		perror("Failed to write the message to the device.\n");
		return errno;
	}
	printf("Reading from the device\n");
	temp = read(fd,buffer,BUFFER_LENGTH);
	if(temp < 0)
	{
		perror("Failed to read the message to the device.\n");
		return errno;
	}
	printf("%s\n",buffer);
	close(fd);
}
