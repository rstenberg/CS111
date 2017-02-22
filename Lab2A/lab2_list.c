#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <getopt.h>
#include <time.h>
#include "SortedList.h"

// Global Variables
int num_threads = 1;
int iterations = 1;
int num_elements = 1;

SortedList_t *list;
SortedList_t **element_array;

int opt_yield = 0;
int mutex_lock_flag = 0;
int spin_lock_flag = 0;

pthread_mutex_t mutex_lock;
int spin_lock = 0;

void performListOperations(int my_id)	{
	// Insert SortedListElements
		for (int i = my_id; i < num_elements; i += num_threads)	{
			SortedList_insert(list, element_array[i]);
		}

		// Get length of list
		int list_length = SortedList_length(list);

		for (int i = my_id; i < num_elements; i += num_threads)	{
			SortedListElement_t *element_to_delete = SortedList_lookup(list, element_array[i]->key);
			if (element_to_delete != NULL)	{
				if (SortedList_delete(element_to_delete) != 0)
					exit(1);
			}
		}
}

long long getNSec(struct timespec start, struct timespec end)	{
	long long start_sec = (long long)(1000000000 * start.tv_sec + start.tv_nsec);
	long long end_sec = (long long)(1000000000 * end.tv_sec + end.tv_nsec);
	return end_sec - start_sec;
}

void initializeEmptyList()	{

	// initialize first element
	list->prev 	= NULL;
	list->next 	= NULL;
	list->key	= NULL;
}

void generateRandomElements(SortedList_t **element_arr, int num_elements)	{

	// Seed random number generator
	srand(time(NULL));

	// 
	char char_set[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	for (int i = 0; i<num_elements; i++)	{
		// allocate memory for single element_arr element
		element_arr[i] = (SortedList_t*)malloc(sizeof(SortedList_t));
		if (element_arr[i] == NULL)	{
			fprintf(stderr, "%s\n", "Error allocating memory!\n");
			exit(1);
		}

		// Set next and prev to NULL
		element_arr[i]->next 	= NULL;
		element_arr[i]->prev 	= NULL;

		// Generate random length and characters for key
		size_t rand_len = (rand() % 10) + 3;
		char *key = (char*)malloc(sizeof(char) * (rand_len + 1));
		if (key==NULL)	{
			fprintf(stderr, "%s\n", "Error allocating memory!\n");
			exit(1);
		}

		for (int j = 0; j<rand_len; j++)	{
			int letter_index = rand() % strlen(char_set);
			key[j] = char_set[letter_index];
		}
		key[rand_len] = '\0';
		element_arr[i]->key 	= key;
	}
}

// Thread function called by each thread
// For each iteration:
//		- Insert random SortedListElement
//	Get length of list
//	For each iteration:
//		- Lookup and delete each key it previously inserted
//	Exit to re-join parent thread
void *thread_function(void *id)	{
	int my_id = *((int*)id);

	if (mutex_lock_flag)	{

		pthread_mutex_lock(&mutex_lock);
		performListOperations(my_id);
		pthread_mutex_unlock(&mutex_lock);

	}	else if (spin_lock_flag)	{

		while(__sync_lock_test_and_set(&spin_lock, 1));
		performListOperations(my_id);
		__sync_lock_release(&spin_lock);

	}	else 	{

		// No lock set
		performListOperations(my_id);
	}

	return NULL;
}

int main(int argc, char *argv[])	{

	// Set default program name
	char* test_name = (char*)malloc(6 * sizeof(char));
	if (test_name==NULL)	{
		fprintf(stderr, "%s\n", "Error allocating memory!\n");
		return -1;
	}
	strcpy(test_name, "list-");

	static struct option long_options[] = 
	{
		{"threads", required_argument, NULL, 't'},
		{"iterations", required_argument, NULL, 'i'},
		{"yield", required_argument, NULL, 'y'},
		{"sync", required_argument, NULL, 's'},
		{0,0,0,0}
	};

	// Parse user options
	int option_index = 0;
	int c;
	while((c = getopt_long(argc, argv, "", long_options, &option_index)) != -1)	{
		switch(c)	{
			case 't':
				num_threads = atoi(optarg);
				break;
			case 'i':
				iterations = atoi(optarg);
				break;
			case 'y':
				if (strchr(optarg, 'i'))	
					opt_yield |= INSERT_YIELD;
				if (strchr(optarg, 'd'))
					opt_yield |= DELETE_YIELD;
				if (strchr(optarg, 'l'))
					opt_yield |= LOOKUP_YIELD;
				break;
			case 's':
				if (strcmp(optarg, "m") == 0)	{
					mutex_lock_flag = 1;
					pthread_mutex_init(&mutex_lock, NULL);
				}	else if (strcmp(optarg, "s") == 0)	{
					spin_lock_flag = 1;
				}
				break;
			default:
				break;
		}
	}

	// Initialize an empty list
	num_elements = num_threads * iterations;
	list  = (SortedList_t*)malloc(sizeof(SortedList_t));
	if (list==NULL)	{
		fprintf(stderr, "%s\n", "Error allocating memory for the empty list!\n");
		return -1;
	}
	initializeEmptyList();

	// Initialize array of randomly generated elements
	element_array  = (SortedList_t**)malloc(sizeof(SortedList_t*) * num_elements);
	if (element_array==NULL)	{
		fprintf(stderr, "%s\n", "Error allocating memory for element array!\n");
		return -1;
	}
	generateRandomElements(element_array, num_elements);

	//Get runtime starting point
	struct timespec start;
	clock_gettime(CLOCK_MONOTONIC, &start);

	// Generate thread ids
	int *ids = (int*)malloc(sizeof(int) * num_threads);
	for (int i = 0; i < num_threads; i++)	{
		ids[i] = i;
	}

	// Create all threads
	pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
	if (threads==NULL)	{
		fprintf(stderr, "%s\n", "Error allocating memory for threads!\n");
		return -1;
	}

	for (int i = 0; i < num_threads; i++)	{
		int result = pthread_create(&threads[i], NULL, &thread_function, &ids[i]) != 0;
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

	// Get length of list
	int list_len = SortedList_length(list);

	// Add yield options to test name
	if (opt_yield & INSERT_YIELD)	{
		test_name = (char*)realloc(test_name, (strlen(test_name) + 2) * sizeof(char));
		strcat(test_name, "i");
	}
	if (opt_yield & DELETE_YIELD)	{
		test_name = (char*)realloc(test_name, (strlen(test_name) + 2) * sizeof(char));
		strcat(test_name, "d");
	}
	if (opt_yield & LOOKUP_YIELD)	{
		test_name = (char*)realloc(test_name, (strlen(test_name) + 2) * sizeof(char));
		strcat(test_name, "l");
	}
	if	(opt_yield == 0)	{
		test_name = (char*)realloc(test_name, (strlen(test_name) + 5) * sizeof(char));
		strcat(test_name, "none");
	}
	test_name = (char*)realloc(test_name, (strlen(test_name) + 2) * sizeof(char));
	strcat(test_name, "-");

	// Add sync options to test name
	if (mutex_lock_flag)	{
		test_name = (char*)realloc(test_name, (strlen(test_name) + 2) * sizeof(char));
		strcat(test_name, "m");
	}	else if (spin_lock_flag)	{
		test_name = (char*)realloc(test_name, (strlen(test_name) + 2) * sizeof(char));
		strcat(test_name, "s");
	}	else	{
		test_name = (char*)realloc(test_name, (strlen(test_name) + 5) * sizeof(char));
		strcat(test_name, "none");
	}

	int num_operations = num_threads * iterations * 3;
	printf("%s,%d,%d,1,%d,%llu,%llu\n", test_name, num_threads, iterations, num_operations, run_nsec, run_nsec/num_operations);

	//Free allocated memory
	free(ids);
	free(element_array);
	free(list);
	free(threads);
	free(test_name);

	exit(0);
}



