#include "Seat.h"
#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROGRAM_ERROR (-1)
#define MAX_SIZE 128

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
