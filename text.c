#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

int fd;

static void SIGAL_DEAL_CODE(int signe)
{
//	static unsigned int cnt =0;

//	printf("sinal: %d, %d\n\r", signe, ++cnt);
	int val =0;

	read(fd, &val, 1);
	printf("0x%x\n\r", val);
}


int main(char argc, char **argv)
{
	int Oflags =0;
	
	fd =open("/dev/key", O_RDWR);
	if(fd < 0)
	{
		printf("cant not open!\n\r");
	}

	fcntl(fd, F_SETOWN, getpid());
	Oflags = fcntl(fd, F_GETFL);
	fcntl(fd, F_SETFL, Oflags | FASYNC);
	
	signal(SIGIO, SIGAL_DEAL_CODE);
	
	while(1)
	{
		sleep(1000);
	}
	return 0;
}


