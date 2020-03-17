#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(char argc, char **argv)
{
	int fd;
	unsigned char val =0;

    fd = open("/dev/key", O_RDWR);
    if (fd < 0)
   	{
       printf("error, can't open\n\r");
    }

	while(1)
	{
		read(fd, &val, 1);
		printf("0x%x\n\r", val);
	}
	return 0;
}


