#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <getopt.h>	// getopt_long
#include <unistd.h>	// exec pipe
#include <sys/types.h>	// wait
#include <sys/wait.h>	// wait

// GLOBAL FLAGS
int vb_flag 		= 0;
int append_flag 	= 0;
int cloexec_flag 	= 0;
int creat_flag 		= 0;
int directory_flag 	= 0;
int dsync_flag 		= 0;
int excl_flag 		= 0;
int nofollow_flag 	= 0;
int nonblock_flag 	= 0;
int rsync_flag 		= 0;
int sync_flag 		= 0;
int trunc_flag 		= 0;

void printUsage()	{
		fprintf(stderr, "%s\n", "simpsh: usage: ./simpsh [--rdonly f] [--wronly f] [--command i o e cmd args] [--verbose]");
		_exit(1);
	}

int startsWith(const char *str1, const char *str2)
{
   if(strncmp(str1, str2, strlen(str2)) == 0) return 1;
   return 0;
}

int getFileFlags()	{
	int flags = 0;

	if (append_flag == 1)	{
		flags = flags | O_APPEND;
	}
	if (cloexec_flag == 1)	{
		flags = flags | O_CLOEXEC;
	}
	if (creat_flag == 1)	{
		flags = flags | O_CREAT;
	}
	if (directory_flag == 1)	{
		flags = flags | O_DIRECTORY;
	}
	if (dsync_flag == 1)	{
		flags = flags | O_DSYNC;
	}
	if (excl_flag == 1)	{
		flags = flags | O_EXCL;
	}
	if (nofollow_flag == 1)	{
		flags = flags | O_NOFOLLOW;
	}
	if (nonblock_flag == 1)	{
		flags = flags | O_NONBLOCK;
	}
	if (rsync_flag == 1)	{
		//flags = flags | O_RSYNC;
	}
	if (sync_flag == 1)	{
		flags = flags | O_SYNC;
	}
	if (trunc_flag == 1)	{
		flags = flags | O_TRUNC;
	}
	return flags;
}

void resetFileFlags()	{
	append_flag 	= 0;
	cloexec_flag 	= 0;
	creat_flag 		= 0;
	directory_flag 	= 0;
	dsync_flag 		= 0;
	excl_flag 		= 0;
	nofollow_flag 	= 0;
	nonblock_flag 	= 0;
	rsync_flag 		= 0;
	sync_flag 		= 0;
	trunc_flag 		= 0;
}

int main(int argc, char *argv[])	{

	char *command 		= NULL;

	static struct option long_options[] = 
	{
		/* These options set a flag. */
		{"append", no_argument, NULL, 'a'},
		{"cloexec", no_argument, NULL, 'l'},
		{"creat", no_argument, NULL, 'm'},
		{"directory", no_argument, NULL, 'd'},
		{"dsync", no_argument, NULL, 'y'},
		{"excl", no_argument, NULL, 'e'},
		{"nofollow", no_argument, NULL, 'n'},
		{"nonblock", no_argument, NULL, 'b'},
		{"rsync", no_argument, NULL, 'x'},
		{"sync", no_argument, NULL, 's'},
		{"trunc", no_argument, NULL, 't'},
		{"rdonly", required_argument, NULL, 'r'},
		{"wronly", required_argument, NULL, 'w'},
		{"rdwr", required_argument, NULL, '&'},
		{"verbose", no_argument, NULL, 'v'},
		/* These options don't set a flag */
		{"command", required_argument, NULL, 'c'},
		{0,0,0,0}
	};

	int c;
	while((c = getopt_long(argc, argv, "", long_options, NULL)) != -1)	{
		switch(c)	{
			case 'a':		// Handle "--append" argument
				if (vb_flag)	{
					printf("--append\n");
				}

				append_flag = 1;
				break;
			case 'l':		// Handle "--cloexec" argument
				if (vb_flag)	{
					printf("--cloexec\n");
				}

				cloexec_flag = 1;
				break;
			case 'm':		// Handle "--creat" argument
				if (vb_flag)	{	
					printf("--creat\n");
				}

				creat_flag = 1;
				break;
			case 'd':		// Handle "--directory" argument
				if (vb_flag)	{	
					printf("--directory\n");
				}

				directory_flag = 1;
				break;
			case 'y':		// Handle "--dsync" argument
				if (vb_flag)	{	
					printf("--dsync\n");
				}

				dsync_flag = 1;
				break;
			case 'e':		// Handle "--excl" argument
				if (vb_flag)	{	
					printf("--excl\n");
				}

				excl_flag = 1;
				break;
			case 'n':		// Handle "--nofollow" argument
				if (vb_flag)	{	
					printf("--nofollow\n");
				}

				nofollow_flag = 1;
				break;
			case 'b':		// Handle "--nonblock" argument
				if (vb_flag)	{	
					printf("--nonblock\n");
				}

				nonblock_flag = 1;
				break;
			case 'x':		// Handle "--rsync" argument
				if (vb_flag)	{	
					printf("--rsync\n");
				}

				rsync_flag = 1;
				break;
			case 's':		// Handle "--sync" argument
				if (vb_flag)	{	
					printf("--sync\n");
				}

				sync_flag = 1;
				break;
			case 't':		// Handle "--trunc" argument
				if (vb_flag)	{	
					printf("--trunc\n");
				}

				trunc_flag = 1;
				break;
			case 'r':		// Handle "--rdonly f" argument

				if (vb_flag)	{
					printf("--rdonly %s\n", optarg);
				}

				int read_file_flags = getFileFlags() | O_RDONLY;

				int rd_fd = open(optarg, read_file_flags);
				if (rd_fd < 0) {
					fprintf(stderr, "Error opening read only file: %s\n", optarg);
					exit(1);
				}

				resetFileFlags();

				break;
			case 'w':		// Handle "--wronly f" argument
				
				if (vb_flag)	{
					printf("--wronly %s\n", optarg);
				}

				int write_file_flags = getFileFlags() | O_WRONLY;

				int wr_fd = open(optarg, write_file_flags);
				if (wr_fd < 0) {
					fprintf(stderr, "Error opening write only file: %s\n", optarg);
					exit(1);
				}

				resetFileFlags();

				break;
			case '&':		// Handle "--rdwr f" argument
				
				if (vb_flag)	{
					printf("--rdwr %s\n", optarg);
				}

				int rdwr_file_flags = getFileFlags() | O_RDWR;

				int rdwr_fd = open(optarg, rdwr_file_flags);
				if (rdwr_fd < 0) {
					fprintf(stderr, "Error opening read-write file: %s\n", optarg);
					exit(1);
				}

				resetFileFlags();

				break;
			case 'c':		// Handle "--command i o cmd args" argument
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
						_exit(1);
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



