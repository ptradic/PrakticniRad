#pragma once

#define MAX_NAME 64

typedef struct Route* RoutePosition;
typedef struct City {
	char CityFrom[MAX_NAME];
	char CityTo[MAX_NAME];
	double RoutePrice;
	int RouteID;
	RoutePosition Next;
}Route;