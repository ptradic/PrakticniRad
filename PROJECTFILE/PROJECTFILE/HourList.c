#define _CRT_SECURE_NO_WARNINGS 

#include "HourList.h"
#include "Seat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int PrintHours(HourPosition head) {

	while (head != NULL) {

		printf(" %s\n", head->startingHour);
		head = head->Next;
	}

	return EXIT_SUCCESS;
}

HourPosition FindHours(HourPosition head, char* hourselect) {

	while (head != NULL) {

		if (strcmp(head->startingHour, hourselect) == 0) {
			system("cls");
			printf("Hour %s selected!\n", head->startingHour);

			return head;
		}

		head = head->Next;
	}

	return NULL;
}

int FreeHourList(HourPosition head) {

	HourPosition temp;

	while (head->Next != NULL) {

		temp = head->Next;
		head->Next = head->Next->Next;
		free(temp);
	}

	return EXIT_SUCCESS;
}

int InsertHours(HourPosition Head, char* filename) {

	char startingHour[10] = { 0 };
	char HourFilename[15] = { 0 };

	HourPosition P = Head;
	HourPosition Q;

	FILE* fp = NULL;
	fp = fopen(filename, "r");

	if (NULL == fp) {

		printf("File didn't open!\r\n");
		return PROGRAM_ERROR;
	}
	while (!feof(fp)) {

		if (fscanf(fp, " %s %s", startingHour, HourFilename) == 2) {

			while (P->Next != NULL) {
				P = P->Next;
			}
			Q = (HourPosition)malloc(sizeof(struct HourList));

			if (Q == NULL) {
				printf("Allocate error!\r\n");
				return PROGRAM_ERROR;
			}

			strcpy(Q->startingHour, startingHour);
			strcpy(Q->HourFilename, HourFilename);

			P->Next = Q;
			Q->Next = NULL;

		}

	}

	fclose(fp);

	return EXIT_SUCCESS;
}