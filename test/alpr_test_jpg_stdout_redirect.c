#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int pipe_fd[2];
	pipe( pipe_fd);

	pid_t pid = fork();

	if( pid > 0)
	{
		printf("Helloow i am parent\n");

		close( pipe_fd[1]);
		char buf[11];
		int end;
		wait();
		while( end = read( pipe_fd[0], buf, 10 ))
		{
			printf("\nparent read:\n");
			buf[end] = '\0';
			printf("%s", buf);
		}
		printf("\nI am finish!!!!!!\n");
	}
	else
	{
		printf( "\nHellow i am child\n%d\n\n", STDERR_FILENO);
		close( pipe_fd[0]);

		dup2( pipe_fd[1], STDERR_FILENO);
		close( pipe_fd[1]);

		execlp("alpr", "alpr", "/home/ubuntu/Taiwan_Province_License_Plate_0146.jpg", NULL);
		//write( STDOUT_FILENO,"\nI am finish~\n", 50);
		exit(0);
	}
	return 0;
}
