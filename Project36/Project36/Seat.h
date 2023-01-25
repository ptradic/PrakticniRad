#pragma once

typedef struct Seat* SeatPosition;
typedef struct Seat {
	char SeatName[4];
	char SeatState[20];
	SeatPosition Next;
}Seat;

int Insert(SeatPosition Head, char* filename, float *price);


