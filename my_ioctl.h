#include<linux/ioctl.h>

typedef struct 
{
	int speed;
	int power;
}device_arg;

#define GET_VARIABLES _IOR('A',1,device_arg*)
#define SET_VARIABLES _IOW('A',2,device_arg*)
#define CLR_VARIABLES _IO('A',3)
