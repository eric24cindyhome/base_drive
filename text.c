#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(char argc, char **argv)
{
	int fd;
	int val =1;
    fd = open("/dev/xxx", O_RDWR);
    if (fd < 0)
   	{
       printf("error, can't open\n\r");
    }
	write(fd, &val, 4);

	return 0;
}


