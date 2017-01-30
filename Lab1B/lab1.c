#define _GNU_SOURCE
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

// Handler to catch and print error message if SIGSEGV encountered
void signal_handler(int sig_num)	{
	fprintf(stderr, "Caught signal with identifier %d", sig_num);
	exit(sig_num);
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

	int main_exit_status = 0;

	int *fileDescriptors = (int*)malloc(sizeof(int));
	int num_of_fd = 0;

	char **commandStrings = (char**)malloc(sizeof(char*));
	int *childProcessPIDs = (int*)malloc(sizeof(int));
	int num_of_childProcesses = 0;

	char *command 		= NULL;

	static struct option long_options[] = 
	{
		/* File flag options. */
		{"append", no_argument, NULL, 'a'},
		{"cloexec", no_argument, NULL, 'l'},
		{"creat", no_argument, NULL, 'm'},
		{"directory", no_argument, NULL, 'd'},
		{"dsync", no_argument, NULL, 'y'},
		{"excl", no_argument, NULL, 'e'},
		{"nofollow", no_argument, NULL, 'n'},
		{"nonblock", no_argument, NULL, 'b'},
		{"rsync", no_argument, NULL, 'q'},
		{"sync", no_argument, NULL, 's'},
		{"trunc", no_argument, NULL, 't'},

		/* File opening options. */
		{"rdonly", required_argument, NULL, 'r'},
		{"wronly", required_argument, NULL, 'w'},
		{"rdwr", required_argument, NULL, '&'},
		{"pipe", no_argument, NULL, 'p'},

		/* Subcommand options. */
		{"command", required_argument, NULL, 'c'},
		{"wait", no_argument, NULL, 'i'},

		/* Miscellaneous options. */
		{"close", required_argument, NULL, 'x'},
		{"verbose", no_argument, NULL, 'v'},
		{"abort", no_argument, NULL, '1'},
		{"catch", required_argument, NULL, '2'},
		{"ignore", required_argument, NULL, '3'},
		{"default", required_argument, NULL, '4'},
		{"pause", no_argument, NULL, '5'},
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
			case 'q':		// Handle "--rsync" argument
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

				// Check for improper argument syntax
				if (startsWith(argv[optind - 1], "--"))	{
					fprintf(stderr, "%s\n", "Improper syntax for --rdonly arguments");
					break;
				}

				if (vb_flag)	{
					printf("--rdonly %s\n", optarg);
				}

				int read_file_flags = getFileFlags() | O_RDONLY;

				int rd_fd = open(optarg, read_file_flags);
				if (rd_fd < 0) {
					fprintf(stderr, "Error opening read only file: %s\n", optarg);
					exit(1);
				}	else	{

					// Ensure that file descriptors are never repeated
					if (rd_fd != num_of_fd + 3)	{
						if (dup2(rd_fd, num_of_fd + 3) == -1) {
							fprintf(stderr, "%s\n", "Error ensuring that file descriptors never get repeated");
							exit(1);
						}	else	{
							if (close(rd_fd) < 0)	{
								fprintf(stderr, "%s\n", "Error: could not close repeating file descriptor");
							}
						}
					}

					num_of_fd += 1;
					fileDescriptors = (int*)realloc(fileDescriptors, (num_of_fd + 1) * sizeof(int));
					fileDescriptors[num_of_fd - 1] = rd_fd;
				}

				resetFileFlags();

				break;
			case 'w':		// Handle "--wronly f" argument

				// Check for improper argument syntax
				if (startsWith(argv[optind - 1], "--"))	{
					fprintf(stderr, "%s\n", "Improper syntax for --wronly arguments");
					break;
				}
				
				if (vb_flag)	{
					printf("--wronly %s\n", optarg);
				}

				int write_file_flags = getFileFlags() | O_WRONLY;

				int wr_fd = open(optarg, write_file_flags);
				if (wr_fd < 0) {
					fprintf(stderr, "Error opening write only file: %s\n", optarg);
					exit(1);
				}	else	{

					// Ensure that file descriptors are never repeated
					if (wr_fd != num_of_fd + 3)	{
						if (dup2(wr_fd, num_of_fd + 3) == -1) {
							fprintf(stderr, "%s\n", "Error ensuring that file descriptors never get repeated");
							exit(1);
						}	else	{
							if (close(wr_fd) < 0)	{
								fprintf(stderr, "%s\n", "Error: could not close repeating file descriptor");
							}
						}
					}

					num_of_fd += 1;
					fileDescriptors = (int*)realloc(fileDescriptors, num_of_fd * sizeof(int));
					fileDescriptors[num_of_fd - 1] = wr_fd;
				}

				resetFileFlags();

				break;
			case '&':		// Handle "--rdwr f" argument

				// Check for improper argument syntax
				if (startsWith(argv[optind - 1], "--"))	{
					fprintf(stderr, "%s\n", "Improper syntax for --rdwr arguments");
					break;
				}
				
				if (vb_flag)	{
					printf("--rdwr %s\n", optarg);
				}

				int rdwr_file_flags = getFileFlags() | O_RDWR;

				int rdwr_fd = open(optarg, rdwr_file_flags);
				if (rdwr_fd < 0) {
					fprintf(stderr, "Error opening read-write file: %s\n", optarg);
					exit(1);
				}	else	{

					// Ensure that file descriptors are never repeated
					if (rdwr_fd != num_of_fd + 3)	{
						if (dup2(rdwr_fd, num_of_fd + 3) == -1) {
							fprintf(stderr, "%s\n", "Error ensuring that file descriptors never get repeated");
							exit(1);
						}	else	{
							if (close(rdwr_fd) < 0)	{
								fprintf(stderr, "%s\n", "Error: could not close repeating file descriptor");
							}
						}
					}

					num_of_fd += 1;
					fileDescriptors = (int*)realloc(fileDescriptors, num_of_fd * sizeof(int));
					fileDescriptors[num_of_fd - 1] = rdwr_fd;
				}

				resetFileFlags();

				break;
			case 'p':		// Handle "--pipe" option

				if (vb_flag)	{
					printf("--pipe\n");
				}

				// Allocate next two available file descriptors
				int myPipe[2] = {num_of_fd, num_of_fd + 1};

				if (pipe(myPipe) == -1)	{
					fprintf(stderr, "%s\n", "Error creating pipe");
				}	else	{
					num_of_fd += 2;
					fileDescriptors = (int*)realloc(fileDescriptors, num_of_fd * sizeof(int));
					fileDescriptors[num_of_fd - 2] = myPipe[0];
					fileDescriptors[num_of_fd - 1] = myPipe[1];
				}

				break;
			case 'c':		// Handle "--command i o cmd args" option/args
				printf("");

				// Check for improper argument syntax
				if (startsWith(argv[optind - 1], "--") ||  startsWith(argv[optind], "--") || startsWith(argv[optind + 1], "--") || startsWith(argv[optind + 2], "--"))	{
					fprintf(stderr, "%s\n", "Improper syntax for --command arguments");
					break;
				}

				// Retrieve the command option's arguments (if any)
				int arg_cnt = 0;
				while(1)	{
					if (argv[optind + 3 + arg_cnt] == NULL || startsWith(argv[optind + 3 + arg_cnt], "--"))	{
						break;
					}	else	{
						arg_cnt += 1;
					}
				}

				int full_command_len = snprintf(NULL, 0, "--command %s %s %s %s", argv[optind - 1], argv[optind], argv[optind + 1], argv[optind + 2]) + 2;
				char* full_command = (char*)malloc(full_command_len * sizeof(char));
				sprintf(full_command, "--command %s %s %s %s", argv[optind - 1], argv[optind], argv[optind + 1], argv[optind + 2]);

				for (int i = 1; i <= arg_cnt; i++)	{
					int arg_len = strlen(argv[optind + 2 + i]) + 2;
					full_command = (char*)realloc(full_command, (strlen(full_command) + arg_len) * sizeof(char));
					full_command = strcat(full_command, argv[optind + 2 + i]);
				}
				
				// Print command option and its arguments if verbose flag set
				if (vb_flag)	{
					printf("%s\n", full_command);
				}

				// Retrieve input opttion
				int input_opt_size = strlen(argv[optind - 1]) + 2;
				char *input_fd = (char*)malloc(input_opt_size * sizeof(char));
				int int_input_fd = atoi(argv[optind - 1]);
				sprintf(input_fd, "%d", int_input_fd);

				// Retrieve output option
				int output_opt_size = strlen(argv[optind]) + 2;
				char *output_fd = (char*)malloc(output_opt_size * sizeof(char));
				int int_output_fd = atoi(argv[optind]);
				sprintf(output_fd, "%d", int_output_fd);

				// Retrieve error option
				int error_opt_size = strlen(argv[optind + 1]) + 2;
				char *error_fd = (char*)malloc(error_opt_size * sizeof(char));
				int int_error_fd = atoi(argv[optind + 1]);
				sprintf(error_fd, "%d", int_error_fd);

				// Retrieve command name
				int command_opt_size = strlen(argv[optind + 2]) + 1;
				char *command = (char*)malloc(command_opt_size * sizeof(char));
				strcpy(command, argv[optind + 2]);

				pid_t pid = fork();
	    		if (pid == 0)	{		// Child process

	    			// Redirect input
					if (dup2(fileDescriptors[int_input_fd], 0) == -1) {
						fprintf(stderr, "Error opening input file descriptor: %s\n", input_fd);
						exit(1);
					}

					//Redirect output
					if (dup2(fileDescriptors[int_output_fd], 1) == -1) {
						fprintf(stderr, "Error opening output file descriptor: %s\n", output_fd);
						exit(1);
					}

					// Redirect error
					if (dup2(fileDescriptors[int_error_fd], 2) == -1) {
						fprintf(stderr, "Error opening error file descriptor: %s\n", error_fd);
						exit(1);
					}

					// Close all other file descriptors
					for (int fd = 3; fd < num_of_fd + 3; fd++)	{
						if (close(fd) < 0)	{
							fprintf(stderr, "Could not close file descriptor %d in child process\n", fd);
						}
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

	    			num_of_childProcesses += 1;
	    			childProcessPIDs = (int*)realloc(childProcessPIDs, num_of_childProcesses * sizeof(int));
	    			childProcessPIDs[num_of_childProcesses - 1] = pid;

	    			commandStrings = (char**)realloc(commandStrings, num_of_childProcesses * sizeof(char*));
	    			commandStrings[num_of_childProcesses - 1] = (char*)malloc(strlen(full_command) * sizeof(char) + 2);
	    			commandStrings[num_of_childProcesses - 1] = full_command;

	    		}	else 	{
	    			fprintf(stderr, "%s\n", "Error forking processes");
	    			exit(1);
	    		}

				break;
			case 'v':	// Handle "--verbose" option
				vb_flag = 1;
				break;
			case 'i':	// Handle "--wait" option

				// Print wait option if verbose flag set
				if (vb_flag)	{
					printf("--wait\n");
				}

				// Close all other file descriptors
				for (int fd = 3; fd < num_of_fd + 3; fd++)	{
					if (close(fd) < 0)	{
						fprintf(stderr, "Could not close file descriptor %d in parent process\n", fd);
					}
				}

				// Wait for all child processes to finish
				for (int i = 0; i < num_of_childProcesses; i++)	{
					int status;
					int child_pid = childProcessPIDs[i];
					if (waitpid(child_pid, &status, 0) != child_pid)	{
						fprintf(stderr, "%s\n", "Error waiting for child process to close");
						exit(1);
					}	else	{
						if (WIFEXITED(status))	{
							int child_exit_status = WEXITSTATUS(status);
							printf("Child process %d exited with status (%d) and command string \"%s\"\n", child_pid, child_exit_status, commandStrings[i]);
							if (child_exit_status > main_exit_status)
								main_exit_status = child_exit_status;
						}
					}
				}

				break;
			case 'x':	// Handle "--close N" 

				// Check for improper argument syntax
				if (startsWith(argv[optind - 1], "--"))	{
					fprintf(stderr, "%s\n", "Improper syntax for --close arguments");
					break;
				}

				char* fd_char = (char*)malloc((strlen(argv[optind - 1]) + 2) * sizeof(char));
				fd_char = argv[optind - 1];
				int fd_int = atoi(argv[optind - 1]);

				// Print close option if verbose flag set
				if (vb_flag)	{
					printf("--close %s\n", fd_char);
				}

				// Attempt to close file 
				if (close(fileDescriptors[fd_int]) < 0)	{
					fprintf(stderr, "Could not close file descriptor %d in parent process\n", fd_int);
					exit(1);
				}

				break;
			case '1':	// Handle "--abort" 
				printf("");

				// Produce segmentation fault
				char *seg = NULL;
				char segChar = *seg;

				break;
			case '2':	// Handle "--catch N"
				
				// Check for improper argument syntax
				if (startsWith(argv[optind - 1], "--"))	{
					fprintf(stderr, "%s\n", "Improper syntax for --catch arguments");
					break;
				}

				int catch_sig = atoi(argv[optind - 1]);
				signal(catch_sig, signal_handler);

				break;
			case '3':	// Handle "--ignore N"
				
				// Check for improper argument syntax
				if (startsWith(argv[optind - 1], "--"))	{
					fprintf(stderr, "%s\n", "Improper syntax for --ignore arguments");
					break;
				}

				int ignore_sig = atoi(argv[optind - 1]);
				signal(ignore_sig, SIG_IGN);

				break;
			case '4':	// Handle "--default N"
				
				// Check for improper argument syntax
				if (startsWith(argv[optind - 1], "--"))	{
					fprintf(stderr, "%s\n", "Improper syntax for --default arguments");
					break;
				}

				int default_sig = atoi(argv[optind - 1]);
				signal(default_sig, SIG_DFL);

				break;
			case '5':	// Handle "--pause"
				printf("");

				pause();

				break;
			default:
				//printUsage();
				break;
		}
	}
	exit(main_exit_status);
}



