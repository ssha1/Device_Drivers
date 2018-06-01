#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<string.h>
#include<fcntl.h>
#include"my_ioctl.h"
#include<sys/mman.h>

#define BUFFER_LENGTH 256

void ioctl_execute(int);
void read_var(int);
void write_var(int);
void clear_var(int);

/*void createFile(char *string)
{
        FILE *fp = fopen("/dev/mynull", "w");
        fprintf(fp, "%s", string);
        fclose(fp);
}

void printFile()
{
        FILE *fp = fopen("/dev/mynull", "r");
        char buf[100];
        if(fgets(buf, 100, fp) == NULL)
        {
                fprintf(stderr, "fgets() error\n");
                exit(EXIT_FAILURE);
        }
        printf("file actually contains contains: %s\n", buf);
        fclose(fp);
}*/


int main()
{
	char* file_name = "/dev/mynull";
	int fd;
	int option;
       	static char device_data[BUFFER_LENGTH];
	char user_data[256];
	int ret;
	int ret_m = 0;
        static char *ch;
	unsigned long int i;
//	createFile("Content of fiel");
	fd = open(file_name,O_RDWR);
	if(fd < 0)
	{
		printf("Cant open file \n");
		return 0;
	}
	ch = mmap(0 , 800 , PROT_READ | PROT_WRITE, MAP_SHARED , fd , 0);
	int k;
	for (k=0; k<800; k++)
		ch[k] = '0';
	i = (unsigned long int)ch;
	if(ch == MAP_FAILED)
	{
		printf("map failed \n");
	}
	printf("PID : %d \n",getpid());
	printf("mmap address : %lx \n",i);
	while(1){
	printf("Choose 1 to read device , 2 to write device , 3 for ioctl commands, 4 for mmap read, 5 for mmap write \n");
	scanf("%d",&option);
	switch(option)
	{
		case 1:
		{
			ret = read(fd,device_data,strlen(device_data));
			printf("Recieved message from device: %s \n",device_data);
			printf("%d \n",ret);
			break;
		}
		case 2:
		{
			printf("Enter the string to send:");
			scanf("%s",user_data);
			ret = write(fd,user_data,strlen(user_data));
			printf("sent %d characters \n",(int)strlen(user_data));
			break;
		}
		case 3:
		{
			ioctl_execute(fd);
			break;
		}
		case 4:
		{
//			ch = mmap(0 , 8192 , PROT_READ , MAP_SHARED , fd , 0);
			if(ch == MAP_FAILED)
			{
			//	perror("mmap");
				printf("map failed");
			}

			printf("%s \n",ch);
//			ret_m = read(fd , ch , strlen(ch));printf("%d \n",ret_m);
			msync(ch , 800 , MS_SYNC);
//			munmap(0 , 8192);
			break;
		}
		case 5:
		{
//			ch =mmap(0 , 8192 , PROT_WRITE | PROT_READ | PROT_EXEC , MAP_SHARED , fd , 0);
			ch = "Hi\0";
			//printf("Enter the string to enter:");
			//scanf("%s",ch);
//			ret_m = write(fd , ch , strlen(ch));printf("%d \n",ret_m);
			printf("%s \n",ch);
			msync(ch , 800, MS_SYNC);			
//			munmap(0 , 8192);
			break;

		}
		case 6:
		{
			munmap(0 , 800);
			close(fd);
			exit(0);
		}
	}}
	munmap(0 ,800);
	close(fd);
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
	darg.speed = var;
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
