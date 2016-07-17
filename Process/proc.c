#include <stdio.h>
#include <string.h>
void proc()
{

	int rate = 0;
	char bar[102];
	memset(bar,'\0',sizeof(bar));

	const char * flag = "|/-\\";
	while(rate <= 100)
	{
		bar[rate] = '=';
		printf("[%-101s][%%%d][%c]\r",bar,rate,flag[rate%4]);
		fflush(stdout);
		rate ++;
		sleep(1);
	}
}

int main()
{
	proc();
	return 0;
}