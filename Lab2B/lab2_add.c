#define _POSIX_C_SOURCE 199309L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <getopt.h>
#include <time.h>

// Global Variables
long long starting_value = 0;
long long *counter 	= &starting_value;
int opt_yield = 0;
int mutex_lock_flag = 0;
int spin_lock_flag = 0;
int sync_val_compare_flag = 0;

pthread_mutex_t mutex_lock;
int spin_lock = 0;
int sync_val_lock = 0;

long long getNSec(struct timespec start, struct timespec end)	{
	long long start_sec = (long long)(1000000000 * start.tv_sec + start.tv_nsec);
	long long end_sec = (long long)(1000000000 * end.tv_sec + end.tv_nsec);
	return end_sec - start_sec;
}

// Function to add 1 and subtract 1
void add(long long *pointer, long long value) {

		// Add 1
        long long sum = *pointer + value;
        if (opt_yield)	{
        	sched_yield();
        } 
        *pointer = sum;
}

void sync_val_compare_and_swap_add(long long *pointer, long long value) {

	long long old_val;
	long long sum;

	do
	{
		old_val = *pointer;
		if (opt_yield)	{
			sched_yield();
		}
		sum = old_val + value;
	} while (__sync_val_compare_and_swap(pointer, old_val, sum) != old_val);
}

// Thread function called by each thread
// Calls the add function based on how many iterations were specified
void *thread_function(void *iterations)	{

	int num_iterations = *((int*)iterations);
	for (int i = 0; i < num_iterations; i++)	{

		if (mutex_lock_flag)	{

			// Use a mutex lock
			pthread_mutex_lock(&mutex_lock);
			add(counter, 1);
			add(counter, -1);
			pthread_mutex_unlock(&mutex_lock);

		}	else if (spin_lock_flag)	{

			// Use a spin lock
			while(__sync_lock_test_and_set(&spin_lock, 1));
			add(counter, 1);
			add(counter, -1);
			__sync_lock_release(&spin_lock);

		}	else if (sync_val_compare_flag)	{

			// Use a sync_val_compare_and_swap
			sync_val_compare_and_swap_add(counter, 1);
			sync_val_compare_and_swap_add(counter, -1);
		}	else 	{
			
			// No lock set
			add(counter, 1);
			add(counter, -1);
		}
	}
}

int main(int argc, char *argv[])	{

	// Set default program name
	char* test_name = (char*)malloc(5 * sizeof(char));
	if (test_name==NULL)	{
		fprintf(stderr, "%s\n", "Error allocating memory!\n");
		return -1;
	}
	strcpy(test_name, "add-");

	//Get runtime starting point
	struct timespec start;
	clock_gettime(CLOCK_MONOTONIC, &start);

	// Thread variables
	int num_threads = 1;
	int iterations = 1;
	int *iteration_ptr = &iterations;

	static struct option long_options[] = 
	{
		{"threads", required_argument, NULL, 't'},
		{"iterations", required_argument, NULL, 'i'},
		{"yield", no_argument, NULL, 'y'},
		{"sync", required_argument, NULL, 's'},
		{0,0,0,0}
	};

	// Parse user options
	int option_index = 0;
	int c;
	while((c = getopt_long(argc, argv, "", long_options, &option_index)) != -1)	{
		switch(c)	{
			case 't':		// Handle "--append" argument
				num_threads = atoi(optarg);
				break;
			case 'i':
				*iteration_ptr = atoi(optarg);
				break;
			case 'y':
				opt_yield = 1;
				break;
			case 's':
				if (strcmp(optarg, "m") == 0)	{
					mutex_lock_flag = 1;
					pthread_mutex_init(&mutex_lock, NULL);
				}	else if (strcmp(optarg, "s") == 0)	{
					spin_lock_flag = 1;
				}	else if (strcmp(optarg, "c") == 0)	{
					sync_val_compare_flag = 1;
				}
				break;
			default:
				break;
		}
	}

	// Create all threads
	pthread_t threads[num_threads];
	for (int i = 0; i < num_threads; i++)	{
		int result = pthread_create(&threads[i], NULL, &thread_function, iteration_ptr) != 0;
		if (result != 0)	{
			fprintf(stderr, "Could not open thread %d. Error code: %d\n", i, result);
			exit(-1);
		}
	}

	// Wait for threads
	for (int i = 0; i < num_threads; i++)	{
		int result = pthread_join(threads[i], NULL);
		if (result != 0)	{
			fprintf(stderr, "Error joining thread: %d. Error code: %d\n", i, result);
			exit(-1);
		}
	}

	// Get runtime ending point
	struct timespec end;
	clock_gettime(CLOCK_MONOTONIC, &end);
	long long run_nsec 	= getNSec(start, end);

	// Set test name to reflect flags
	if (opt_yield)	{
		int size = strlen(test_name) + 7;
		test_name = (char*)realloc(test_name, size * sizeof(char));
		if (test_name==NULL)	{
			fprintf(stderr, "%s\n", "Error allocating memory!\n");
			return -1;
		}
		strcat(test_name, "yield-");
	}

	if (mutex_lock_flag)	{
		test_name = (char*)realloc(test_name, (strlen(test_name) + 2) * sizeof(char));
		strcat(test_name, "m");
	}	else if (spin_lock_flag)	{
		test_name = (char*)realloc(test_name, (strlen(test_name) + 2) * sizeof(char));
		strcat(test_name, "s");
	}	else if (sync_val_compare_flag)	{
		test_name = (char*)realloc(test_name, (strlen(test_name) + 2) * sizeof(char));
		strcat(test_name, "c");
	}	else	{
		test_name = (char*)realloc(test_name, (strlen(test_name) + 5) * sizeof(char));
		strcat(test_name, "none");
	}

	int num_operations = num_threads * iterations * 2;
	printf("%s,%d,%d,%d,%llu,%llu,%llu\n", test_name, num_threads, iterations, num_operations, run_nsec, run_nsec/num_operations, *counter);

	return 0;
}


