#pragma once

#include "LogIn.h"

typedef struct Seat* SeatPosition;
typedef struct Seat {
	char SeatName[4];
	char SeatState[20];
	SeatPosition Next;
}Seat;

int Insert(SeatPosition Head,char* filename,float *price);
int RewriteFile(SeatPosition head, char* filename, float price);
int PrintSeat(SeatPosition temp);
float percentageFull(SeatPosition head);
int FreeList(SeatPosition head);
