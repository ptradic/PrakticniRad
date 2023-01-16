#include "Seat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROGRAM_ERROR (-1)
#define MAX_SIZE 128

int Insert(Position Head) {
	char SeatName = "", SeatState = "";

	Position P = Head;
	Position Q;

	FILE* fp = NULL;

	fp = fopen("Seat.txt", "r");

	if (NULL == fp) {
		printf("File didn't open!\r\n");
		return PROGRAM_ERROR;
	}

	while (!feof(fp)) {

		fscanf(fp, " %s %s", &SeatName, &SeatState);

		while (P->Next != NULL) {
			P = P->Next;
		}

		Q = (Position)malloc(sizeof(struct Seat));

		if (Q == NULL) {
			printf("Allocate error!\r\n");
			return PROGRAM_ERROR;
		}

		P->SeatName = SeatName;
		P->SeatState = SeatState;

		P->Next = Q;
		Q->Next = NULL;
	}
	

	fclose(fp);

	return EXIT_SUCCESS;
}

