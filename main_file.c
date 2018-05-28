#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<string.h>
#include<fcntl.h>
#include"my_ioctl.h"

#define BUFFER_LENGTH 256
int main()
{
	char* file_name = "/dev/my_device";
	int fd;
	int option;
	char device_data[BUFFER_LENGTH];
	char* user_data = "User data";
	int ret;
	fd = open(file_name,O_RDWR);
	printf("Choose 1 to read device , 2 to write device , 3 for ioctl commands");
	scanf("%d",&option);
	switch(option)
	{
		case 1:
		{
			ret = read(fd,device_data,BUFFER_LENGTH);
			printf("Recieve message: %s containing %d characters",device_data,ret);
		}
		case 2:
		{
			write(fd,user_data,strlen(user_data));
		}
		case 3:
		{
			
		}
	}
	return 0;	
}
