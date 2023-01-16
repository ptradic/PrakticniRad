#pragma once

typedef struct Seat* Position;
typedef struct Seat {
	char SeatName;
	char SeatState;
	Position Next;
}Seat;

int Insert(Position Head);