#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "SortedList.h"

void SortedList_insert(SortedList_t *list, SortedListElement_t *element)	{

	// Check for NULL list/element
	if (list == NULL | element == NULL)	{
		fprintf(stderr, "%s\n", "Error: NULL list or element");
		exit(1);
	}

	SortedListElement_t *ptr = list->next;

	// Check for empty list
	if (ptr == NULL)	{
		list->next = element;
		element->prev = list;
		return;
	}

	// Search for position in list to insert
	while (1)	{
		if (strcmp(ptr->key, element->key) > 0)	{

			// element has a lower key value than ptr -- insert before
			element->prev	= ptr->prev;
			element->next 	= ptr;
			ptr->prev->next	= element;
			if (opt_yield & INSERT_YIELD) {
				sched_yield();
			}
			ptr->prev 		= element;
			return;
		}

		if (ptr->next == NULL)	{
			// Reached end of list (append)
			ptr->next = element;
			element->prev = ptr;
			return;
		}	else
			ptr = ptr->next;
	}
	return;
}





int SortedList_delete(SortedListElement_t *element)	{

	if (element->prev != NULL && element->next == NULL)	{
		// Found last element in list --> attempt to delete it
		if (element->prev->next == element)	{
			element->prev->next = NULL;
			if (opt_yield & DELETE_YIELD) {
				sched_yield();
			}
			element->prev = NULL;
			return 0;
		}	else	{
			fprintf(stderr, "%s\n", "Error: Corrupted prev->next pointer!");
			return 1;
		}
	}	else	if (element->prev != NULL && element->next != NULL)	{
		// Found element that is surrounded by other elements --> attempt to delete it
		if (element->next->prev == element && element->prev->next == element)	{

			// Delete element
			element->prev->next = element->next;
			if (opt_yield & DELETE_YIELD) {
				sched_yield();
			}
			element->next->prev = element->prev;
			return 0;

		}	else	{
			// Corrupted prev/next pointers
			fprintf(stderr, "%s\n", "Corrupted prev->next or next->prev pointer during SortedList_delete");
			return 1;
		}
	}
	return 1;
}





SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key)	{

	// Check for NULL list/element
	if (list == NULL | key == NULL)	{
		fprintf(stderr, "%s\n", "Error: NULL list or key during lookup");
		exit(1);
	}

	SortedListElement_t *ptr = list->next;

	// Check for empty list
	if (ptr == NULL)	{
		return NULL;
	}	else	{

		while(ptr != NULL)	{
			if (strcmp(ptr->key, key) == 0)	{
				if (opt_yield & LOOKUP_YIELD) {
					sched_yield();
				}
				return ptr;
			}
			ptr = ptr->next;
		}
		return NULL;

	}
}





int SortedList_length(SortedList_t *list)	{

	// Check for NULL list/element
	if (list == NULL)	{
		fprintf(stderr, "%s\n", "Error: NULL list during get length");
		exit(1);
	}

	SortedListElement_t *ptr = list->next;

	int counter = 0;
	while(ptr != NULL)	{
		int cnt = counter;
		if (opt_yield & LOOKUP_YIELD) {
			sched_yield();
		}
		counter = cnt + 1;
		ptr = ptr->next;
	}
	return counter;
}


