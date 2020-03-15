#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(char argc, char **argv)
{
	int fd;
	int val =0;

    fd = open("/dev/led", O_RDWR);
    if (fd < 0)
   	{
       printf("error, can't open\n\r");
    }
	
	if(argc != 2)
	{
		printf("./text <on/off>\n\r");
		return 0;
	}
	
	if(strcmp(argv[1], "on") == 0)
	{
		val =1;
	}
	else
	{
		val =2;
	}
	
	write(fd, &val, 4);

	return 0;
}


