#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>

void printUsage()	{
	fprintf(stderr, "%s\n", "lab0: usage: ./lab0 [--input=<filename>] [--output=<filename>] [--segfault] [--catch]");
	exit(1);
}

// Handler to catch and print error message if SIGSEGV encountered
void sigsegv_handler(int sig_num)	{
	if (sig_num == SIGSEGV)	{
		perror("Error");
		exit(3);
	}
}

int main(int argc, char *argv[])	{

	char* input;
	char* output;
	int segFlag = 0;
	int catchFlag = 0;

	static struct option long_options[] = 
	{
		/* These options set a flag. */
		{"segfault", no_argument, NULL, 's'},
		{"catch", no_argument, NULL, 'c'},
		/* These options don't set a flag */
		{"input", required_argument, NULL, 'i'},
		{"output", required_argument, NULL, 'o'},
	};

	int c;
	while((c = getopt_long(argc, argv, "", long_options, NULL)) != -1)
	{
		switch(c)	{
			case 'i':
				input = optarg;
				break;
			case 'o':
				output = optarg;
				break;
			case 's':
				segFlag = 1;
				break;
			case 'c':
				signal(SIGSEGV, sigsegv_handler);
				break;
			default:
				printUsage();
				exit(1);
		}
	}

	// Redirect input if necessary
	if (input != NULL)	{
		int ifd = open(input, O_RDONLY, 0);
		if (ifd >= 0) {
			close(0);
			dup(ifd);
			close(ifd);
		}	else	{
			fprintf(stderr, "%s\n", "Error opening input file");
			perror("Error");
			exit(1);
		}
	}

	// Redirect output if necessary
	if (output != NULL) 	{	
		int ofd = creat(output, 0666);
		if (ofd >= 0) {
			close(1);
			dup(ofd);
			close(ofd);
		}	else	{
			fprintf(stderr, "%s\n", "Error opening output file");
			perror("Error");
			exit(2);
		}
	}

	// Produce segmentation fault if segFlag was set
	if (segFlag)	{
		char *seg = NULL;
		char segChar = *seg;
	}

	// Read from input file and write to output file
	char cur_char;
	while(1)	{
		ssize_t read_status = read(0, &cur_char, 1);
		if (read_status)	{
			write(1, &cur_char, 1);
		}	else	{
			exit(0);
		}
	}
	
}