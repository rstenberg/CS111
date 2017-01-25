#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <getopt.h>	// getopt_long
#include <unistd.h>	// exec pipe
#include <sys/types.h>	// wait
#include <sys/wait.h>	// wait

void printUsage()	{
		fprintf(stderr, "%s\n", "simpsh: usage: ./simpsh [--rdonly f] [--wronly f] [--command i o e cmd args] [--verbose]");
		_exit(1);
	}

int startsWith(const char *str1, const char *str2)
{
   if(strncmp(str1, str2, strlen(str2)) == 0) return 1;
   return 0;
}

int main(int argc, char *argv[])	{

	char *command = NULL;
	int vb_flag = 0;

	static struct option long_options[] = 
	{
		/* These options set a flag. */
		{"rdonly", required_argument, NULL, 'r'},
		{"wronly", required_argument, NULL, 'w'},
		{"verbose", no_argument, NULL, 'v'},
		/* These options don't set a flag */
		{"command", required_argument, NULL, 'c'},
		{0,0,0,0}
	};

	int c;
	while((c = getopt_long(argc, argv, "", long_options, NULL)) != -1)	{
		switch(c)	{
			case 'r':

				if (vb_flag)	{
					printf("--rdonly %s\n", optarg);
				}

				int ifd = open(optarg, O_RDONLY);
				if (ifd < 0) {
					fprintf(stderr, "Error opening read only file: %s\n", optarg);
					exit(1);
				}

				break;
			case 'w':
				
				if (vb_flag)	{
					printf("--wronly %s\n", optarg);
				}

				int ofd = open(optarg, 0666);
				if (ofd < 0) {
					fprintf(stderr, "Error opening write only file: %s\n", optarg);
					exit(1);
				}

				break;
			case 'c':
				printf("");

				// Retrieve the command option's arguments (if any)
				int arg_cnt = 0;
				while(1)	{
					if (argv[optind + 3 + arg_cnt] == NULL || startsWith(argv[optind + 3 + arg_cnt], "--"))	{
						break;
					}	else	{
						arg_cnt += 1;
					}
				}

				// Print command option and its arguments if verbose flag set
				if (vb_flag)	{

					// Print required command arguments
					printf("--command %s %s %s %s", argv[optind - 1], argv[optind], argv[optind + 1], argv[optind + 2]);

					// Print optional command arguments (depends on what command is being executed)
					for (int i = 1; i <= arg_cnt; i++)	{
						printf(" %s", argv[optind + 2 + i]);
					}
					printf("\n");
				}

				// Retrieve input opttion

				int input_opt_size = strlen(argv[optind - 1]) + 2;
				char *input_fd = (char*)malloc(input_opt_size * sizeof(char));
				int int_input_fd = atoi(argv[optind - 1]) + 3;
				sprintf(input_fd, "%d", int_input_fd);

				// Retrieve output option
				int output_opt_size = strlen(argv[optind]) + 2;
				char *output_fd = (char*)malloc(output_opt_size * sizeof(char));
				int int_output_fd = atoi(argv[optind]) + 3;
				sprintf(output_fd, "%d", int_output_fd);

				// Retrieve error option
				int error_opt_size = strlen(argv[optind + 1]) + 2;
				char *error_fd = (char*)malloc(error_opt_size * sizeof(char));
				int int_error_fd = atoi(argv[optind + 1]) + 3;
				sprintf(error_fd, "%d", int_error_fd);

				// Retrieve command name
				int command_opt_size = strlen(argv[optind + 2]) + 1;
				char *command = (char*)malloc(command_opt_size * sizeof(char));
				strcpy(command, argv[optind + 2]);

				pid_t pid = fork();
	    		if (pid == 0)	{
	    			// Child process

	    			// Redirect input
					if (dup2(int_input_fd, 0) == -1) {
						fprintf(stderr, "Error opening input file descriptor: %s\n", input_fd);
						exit(1);
					}

					//Redirect output
					if (dup2(int_output_fd, 1) == -1) {
						fprintf(stderr, "Error opening output file descriptor: %s\n", output_fd);
						exit(1);
					}

					// Redirect error
					if (dup2(int_error_fd, 2) == -1) {
						fprintf(stderr, "Error opening error file descriptor: %s\n", error_fd);
						exit(1);
					}

					char **exec_argv;
					exec_argv = (char**)malloc((arg_cnt + 2) * sizeof(char*));
					for (int i = 0; i <= arg_cnt; i++)	{
						exec_argv[i] = (char*)malloc((strlen(argv[optind + 2 + i]) + 1) * sizeof(char));
						exec_argv[i] = argv[optind + 2 + i];
					}
					exec_argv[arg_cnt + 1] = NULL;
					if (execvp(command, exec_argv) == -1)	{
						fprintf(stderr, "Error executing %s with execvp()\n", command);
						exit(1);
					}

	    			// Execvp should exit the child process
	    		}	else if (pid > 0)	{
	    			printf("%s\n", "Parent process here");
	    			break;
	    		}	else 	{
	    			printf("Error forking processes");
	    			exit(1);
	    		}

				break;
			case 'v':
				vb_flag = 1;
				break;
			default:
				//printUsage();
				break;
		}
	}

}



