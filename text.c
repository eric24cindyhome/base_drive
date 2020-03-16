#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(char argc, char **argv)
{
	int fd;
	unsigned int count =0;
	unsigned char val[4] ={0};

    fd = open("/dev/key", O_RDWR);
    if (fd < 0)
   	{
       printf("error, can't open\n\r");
    }

	while(1)
	{
		read(fd, val, sizeof(val));
		if(!val[0] || !val[1] || !val[2] || !val[3])
		{
			count++;
			printf("count= %03d, %d%d%d%d\n\r", count, val[0], val[1], val[2], val[3]);
		}
	}
	return 0;
}


