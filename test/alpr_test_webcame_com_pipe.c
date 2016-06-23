#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int pipe_h2d[2];
	int pipe_d2h[2];
	pipe( pipe_h2d);
	pipe( pipe_d2h);
	/*pipe[0] for read
	  pipe[1] for write*/

	pid_t pid = fork();

	if( pid > 0)
	{
		printf("Parent Ready!!! Wait for Child\n");

		close( pipe_h2d[0]);
		close( pipe_d2h[1]);
		int i=0;
		while(1)
		{
			getchar();
			printf("\ntimes %d\n", i);
			
			int end = 0;
			char buf[50];
			char c[1];
			c[1] = 'm';
			write( pipe_h2d[1], c, sizeof(char));
			printf("\nwrite to alpr success\n");

			while(1)
			{
				if( end = read( pipe_d2h[0], buf, 50))
				{
					printf("end=%d", end);
					if(end <6)
					{
						write( pipe_h2d[1], c, sizeof(char));
						printf("keyspace: Enter");
						sleep(0.5);
					}
					else
						break;
				}
				/*if(end > 5){
					buf[end+1] = '\0';
					printf("end=%s", buf);
				}*/
				sleep(0.5);
				
			}
			buf[end+1] = '\0';
			printf("\n\nparent get: %s\n\n", buf);
			i++;
		}
	}
	else
	{
		printf("Child Process Ready!!!\n");
		close( pipe_h2d[1]);
		close( pipe_d2h[0]);

		dup2( pipe_h2d[0], STDIN_FILENO);
		dup2( pipe_d2h[1], STDERR_FILENO);

		close( pipe_h2d[0]);
		close( pipe_d2h[1]);

		execlp("alpr", "alpr", "-n 1","/dev/video0", NULL);

		exit(0);
	}
	return 0;
}
