#define _CRT_SECURE_NO_WARNINGS 
#include "Seat.h"
#include "LogIn.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROGRAM_ERROR (-1)
#define MAX_SIZE 128

int Insert(SeatPosition Head, char* filename) {
	char SeatName[4] = { 0 }, SeatState[20] = { 0 };

	SeatPosition P = Head;
	SeatPosition Q;

	FILE* fp = NULL;

	fp = fopen(filename, "r");

	if (NULL == fp) {
		printf("File didn't open!\r\n");
		return PROGRAM_ERROR;
	}
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

int RewriteFile(SeatPosition head, char* filename) {
	FILE* fp = NULL;
	fp = fopen(filename, "w+");
	int counter = 0;
	if (NULL == fp) {
		perror("Error opening file!");
		return -1;
	}
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

int TakeSeat(SeatPosition head, char* SeatChoice, Position loggedIn) {
	while (head != NULL) {
		if (strcmp(head->SeatName, SeatChoice) == 0 && strcmp(head->SeatState, "<Empty>") == 0) {
			strcpy(head->SeatState, loggedIn->username);
			printf("Seat is available and is now yours!\n");
			return EXIT_SUCCESS;
		}
		else {
			head = head->Next;
		}
	}
	printf("\nThat seat is not available!\n");
	return -2;
}
