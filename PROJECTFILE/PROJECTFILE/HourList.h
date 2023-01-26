#pragma once

typedef struct HourList* HourPosition;
typedef struct HourList {
	char startingHour[15];
	char HourFilename[20];
	HourPosition Next;
}HourList;


int PrintHours(HourPosition head);
HourPosition FindHours(HourPosition head, char* timeselect);
int FreeHourList(HourPosition head);
int InsertHours(HourPosition Head, char* filename);
