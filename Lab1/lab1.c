#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>	// getopt_long
#include <unistd.h>	// exec pipe
#include <sys/types.h>	// wait
#include <sys/wait.h>	// wait

void printUsage()	{
		fprintf(stderr, "%s\n", "simpsh: usage: ./simpsh [--rdonly f] [--wronly f] [--command i o e cmd args] [--verbose]");
		_exit(1);
	}

int main(int argc, char *argv[])	{

	int rd_flag = 0;
	int wr_flag = 0;
	char *command = NULL;
	int vb_flag = 0;

	static struct option long_options[] = 
	{
		/* These options set a flag. */
		{"rdonly", no_argument, NULL, 'r'},
		{"wronly", no_argument, NULL, 'w'},
		{"verbose", no_argument, NULL, 'v'},
		/* These options don't set a flag */
		{"command", required_argument, NULL, 'c'},
		{0,0,0,0}
	};

	int c;
	while((c = getopt_long(argc, argv, "", long_options, NULL)) != -1)	{
		switch(c)	{
			case 'r':
				printf("%s\n", "R_ONLY");
				rd_flag = 1;
				break;
			case 'w':
				printf("%s\n", "W_ONLY");
				wr_flag = 1;
				break;
			case 'c':
				printf("%s\n", "COMMAND");
				command = optarg;
				break;
			case 'v':
				printf("%s\n", "VERBOSE");
				vb_flag = 1;
				break;
			default:
				printUsage();
				break;
		}
	}

}



