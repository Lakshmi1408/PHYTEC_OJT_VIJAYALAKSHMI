#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



int main()
{
	int fd;
	int i=20;
	
	fd=open("/dev/my_led",0666);
	
	while(i--)
	{
		write(fd,"on",3);
		printf("led on\n");
		sleep(2);
		
		write(fd,"off",4);
		printf("led off\n");
		sleep(2);
	}			

	close(fd);
	return 0;
}
