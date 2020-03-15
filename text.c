#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(char argc, char **argv)
{
	int fd;
	int val =1;

	if(argc != 3)
	{
		printf("./text <path> <on/off>\n\r");
		return 0;
	}
    fd = open(argv[1], O_RDWR);
    if (fd < 0)
   	{
       printf("error, can't open\n\r");
    }

	if(strcmp(argv[2], "on") == 0)
	{
		val = 1;
	}
	else
	{
		val = 0;
	}
	write(fd, &val, 4);

	return 0;
}


