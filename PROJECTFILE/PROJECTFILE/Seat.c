#define _CRT_SECURE_NO_WARNINGS 

#include "Seat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Insert(SeatPosition Head, char* filename,float *price) {

	char SeatName[4] = { 0 }, SeatState[20] = { 0 };

	SeatPosition P = Head;
	SeatPosition Q;

	FILE* fp = NULL;
	fp = fopen(filename, "r");

	if (NULL == fp) {
		printf("File didn't open!\r\n");
		return PROGRAM_ERROR;
	}

	fscanf(fp, "%f", price);

	while (!feof(fp)) {

		if (fscanf(fp, " %s %s", SeatName, SeatState) == 2) {

			while (P->Next != NULL) {
				P = P->Next;
			}

			Q = (SeatPosition)malloc(sizeof(struct Seat));

			if (Q == NULL) {
				printf("Allocate error!\r\n");
				return PROGRAM_ERROR;
			}

			strcpy(Q->SeatName, SeatName);
			strcpy(Q->SeatState, SeatState);

			P->Next = Q;
			Q->Next = NULL;
		}
	}

	fclose(fp);

	return EXIT_SUCCESS;
}

int PrintSeat(SeatPosition temp) {

	int counter = 0;

	while (temp->Next != NULL) {

		printf(" %-4s %-20s ", temp->Next->SeatName, temp->Next->SeatState);
		temp = temp->Next;
		counter++;

		if (counter % 4 == 0) {
			printf("\n");
		}
	}

	return EXIT_SUCCESS;
}

float percentageFull(SeatPosition head) {

	int counter = 0;
	int taken = 0;

	while (head->Next != NULL) {

		if (strcmp(head->SeatState, "<Empty>") != 0) {
			taken++;
		}

		counter++;
		head = head->Next;

	}

	return ((float)taken / counter) * 100;
}

int FreeList(SeatPosition head) {

	SeatPosition temp;

	while (head->Next != NULL) {

		temp = head->Next;
		head->Next = head->Next->Next;
		free(temp);
	}

	return EXIT_SUCCESS;
}

int RewriteFile(SeatPosition head, char* filename, float price) {

	int counter = 0;

	FILE* fp = NULL;
	fp = fopen(filename, "w+");

	if (NULL == fp) {

		perror("Error opening file!");
		return PROGRAM_ERROR;
	}

	fprintf(fp, "%.2f \n", price);

	while (head->Next != NULL) {

		fprintf(fp, "%s %s ", head->Next->SeatName, head->Next->SeatState);
		head = head->Next;
		counter++;

		if (counter % 4 == 0) {
			fprintf(fp, "\n");
		}

	}

	fclose(fp);

	return EXIT_SUCCESS;
}

