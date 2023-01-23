#pragma once

#define MAX_NAME 64
#define MAX_MARK 2

typedef struct City* CityPosition;
typedef struct City {
	char CityName[MAX_NAME];
	char CityMark[MAX_MARK];
	int CityID;
	CityPosition left;
	CityPosition right;
}City;