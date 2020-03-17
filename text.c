#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>

int main(char argc, char **argv)
{
	int fd;
	unsigned char val =0;
	int res =0;
	struct pollfd fds;

    fd = open("/dev/key", O_RDWR);
    if (fd < 0)
   	{
       printf("error, can't open\n\r");
    }

	fds.fd =fd;
	fds.events =POLLIN;

	while(1)
	{
		res =poll(&fds, 1, 5000);
		if(res == 0)
		{
			printf("timeout!wait...\n\r");
		}
		else
		{
			read(fd, &val, 1);
			printf("0x%x\n\r", val);
		}
	}
	return 0;
}


