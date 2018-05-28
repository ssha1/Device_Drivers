#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<string.h>
#include<fcntl.h>
#include"my_ioctl.h"

#define BUFFER_LENGTH 256

void ioctl_execute(int);
void read_var(int);
void write_var(int);
void clear_var(int);

int main()
{
	char* file_name = "/dev/mynull";
	int fd;
	int option;
       	static char device_data[BUFFER_LENGTH];
	char* user_data = "User data";
	int ret;
	fd = open(file_name,O_RDWR);
	if(fd < 0)
	{
		printf("Cant open file \n");
		return 0;
	}
	printf("Choose 1 to read device , 2 to write device , 3 for ioctl commands \n");
	scanf("%d",&option);
	switch(option)
	{
		case 1:
		{
			ret = read(fd,device_data,BUFFER_LENGTH);
			printf("Recieve message: %s containing %d characters \n",device_data,((int)strlen(device_data)));
			break;
		}
		case 2:
		{
			ret = write(fd,user_data,strlen(user_data));
			printf("sent %d characters \n",ret);
			break;
		}
		case 3:
		{
			ioctl_execute(fd);
			break;
		}
	}
	return 0;	
}

void ioctl_execute(int fd)
{
	int choice;
	printf("Enter 1 to get variables, 2 to set variables, 3 to clear variables");
	scanf("%d",&choice);
	switch(choice) 
	{
		case 1:
			read_var(fd);
			break;
		case 2:
			write_var(fd);
			break;
		case 3:
			clear_var(fd);
			break;
		default:
			printf("Enter valid choice");
	}
}

void read_var(int fd)
{
	device_arg darg; 
	if(ioctl(fd,GET_VARIABLES,&darg)==-1)
	{
		perror("ioctl get variable call failed");
	}
	else
	{
		printf("Power: %d\n",darg.power);
		printf("Speed: %d\n",darg.speed);
	}
}

void write_var(int fd)
{
	device_arg darg;
	int var;
	printf("Enter power : ");
	scanf("%d",&var);
	darg.power = var;
	printf("Enter Speed : ");
	scanf("%d",&var);
	darg.power = var;
	if(ioctl(fd,SET_VARIABLES,&darg)==-1)
	{
		perror("ioctl set variable call failed");
	}
}

void clear_var(int fd)
{
	if(ioctl(fd,CLR_VARIABLES)==-1)
	{
		perror("ioctl clear variable call failed");
	}
}
