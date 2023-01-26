#define _CRT_SECURE_NO_WARNINGS

#include "LogIn.h"
#include "Seat.h"
#include "HourList.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINE 1024
#define NUMBER_OF_ROUTES 5	


int ReservationMenu(Position loggedIn);
int RewriteFile(SeatPosition head, char* filename,float price);
int TakeSeat(SeatPosition head, char* SeatChoice, Position loggedIn);
int AdminView(Position loggedIn);
float AdjustedPrice(float percentageFull, float price);

int main() {

	BinarySearchTree headAccount = { .username = " ",.password = "",.right = NULL,.left = NULL };
	Position head = &headAccount;
	Position loggedIn = NULL;
	LoadAccountsFromFile(head);
	char choice = '0';
	do {
		system("cls");
		printf("=========================================================================\n");
		printf("Welcome to bus reservation system! \nMenu: \n");
		printf("1-Register a new account\n2-Login\n0-Exit\n ");
		printf("=========================================================================\n");
		printf("Choose your next action: ");
		scanf_s(" %c", &choice, MAX_SIZE);
		switch (choice) {
		case '1':
			system("cls");
			printf("->Account creation page\n");
			AddAccountToFile(head);
			break;
		case '2':
			system("cls");
			loggedIn = LogIn(head);
			if (loggedIn != NULL) {
				if (strcmp(loggedIn->username, "admin") == 0) {
					AdminView(loggedIn);
				}
				else {
					ReservationMenu(loggedIn);
				}
			} //printfovi samo da se vidi da je na tocnom useru i sad ulazi u glavni meni di bi tribali rezervirat i ostalo 
			break;
		case '0':
			break;
		default:
			printf("Wrong input!");
			break;
		}
	} while (choice != '0');
	DeleteAll(head->left);
	DeleteAll(head->right);
	return EXIT_SUCCESS;
}
int AdminView(Position loggedIn) {
	system("cls");
	printf("logged in as -> %s \n", loggedIn->username);
	printf("Current status of all active busses:\n");
	char BusArray[NUMBER_OF_ROUTES][10] = { 0 };
	char filetemp[NUMBER_OF_ROUTES][10] = { 0 };
	FILE* fp = NULL;
	int i = 0;
	HourList hourAdmin = { .HourFilename = "",.startingHour = "",.Next = NULL };
	HourPosition headAdminHead = &hourAdmin;
	HourPosition temp = headAdminHead;
	Seat seatAdmin = { .SeatName = "",.SeatState = "",.Next = NULL };
	SeatPosition seatAdminHead = &seatAdmin;
	float price = 0;
	float percentage = 0;
	char choice[5] = "";
	fp = fopen("autobusi.txt","r");
	if (!fp) {
		printf("error opening file!");
		return -1;
	}
	while (!feof(fp)) {
		fscanf(fp, "%s %s", BusArray[i], filetemp[i]);
		i++;
	}
	fclose(fp);
	for (int i = 0; i < NUMBER_OF_ROUTES; i++) {
		printf("\n");
		printf("Statistics for route: %s\n", BusArray[i]);
		InsertHours(headAdminHead, filetemp[i]);
		temp = headAdminHead;
		while (temp->Next != NULL) {
			Insert(seatAdminHead, temp->Next->HourFilename, &price);
			percentage = percentageFull(seatAdminHead->Next);
			printf("Hour %s is at-> %-5.2f%% capacity, current price ->%.2f (starting price was %.2f)\n", temp->Next->startingHour, percentage, AdjustedPrice(percentage,price), price);
			//PrintSeat(seatAdminHead);
			FreeList(seatAdminHead);
			temp = temp->Next;
		}
		FreeHourList(headAdminHead);
	}

	printf("Press any key to continue...\n");
	getch();
	return EXIT_SUCCESS;
}
float AdjustedPrice(float percentageFull,float price) {
	if (percentageFull > 80) {
		return price * 1.4;
	}
	else if (percentageFull > 60) {
		return price * 1.25;
	}
	else if (percentageFull > 50) {
		return price * 1.1;
	}
	return price;
}
int ReservationMenu(Position loggedIn) {
	FILE* fp = NULL;
	fp = fopen("autobusi.txt", "r");
	char BusArray[NUMBER_OF_ROUTES][10] = { 0 };
	char filetemp[NUMBER_OF_ROUTES][10] = { 0 };
	int i = 0;
	int choice = 0;
	Seat SeatHead = { .SeatName = "",.SeatState = "" ,.Next = NULL };
	SeatPosition head= &SeatHead;
	HourList Head = { .HourFilename = "",.startingHour = "",.Next = NULL };
	HourPosition hourHead = &Head;
	char SeatChoice[5] = { 0 };
	float price = 0;
	int numberofTickets = 0;
	char timeselect[10] = { 0 };
	HourPosition selected;
	char proceed[10] = { 0 };
	float percentage = 0;
	if (!fp) {
		perror("Error opening bus file!");
		return -1;
	}
	while (!feof(fp)) {
		fscanf(fp, "%s %s", BusArray[i], filetemp[i]);
		i++;
	}
	fclose(fp);
	do {
		system("cls");
		printf("logged in as-> %s \n", loggedIn->username);
		for (int i = 0; i < NUMBER_OF_ROUTES; i++) {
			printf("%d %s\n", i + 1, BusArray[i]); //samo za pracenje da je dobro zapisano
		}
		printf("Choose your route or press 0 to exit: \n");
		scanf(" %d", &choice);
		if (choice == 0) {
			printf("Exited to main menu!\n");
			break;
		}
		if (choice <= NUMBER_OF_ROUTES) {
			InsertHours(hourHead, filetemp[choice - 1]);
			system("cls");
			printf("These are the available starting horus for the chosen route: \n");
			PrintHours(hourHead->Next);
			do{
				printf("Enter the time at which you would like to travel: ");
			scanf(" %s", timeselect);
			} while ((selected=FindHours(hourHead, timeselect)) == NULL);
			Insert(head, selected->HourFilename, &price); //napravi listu 
			printf("The price of a ticket on this line is: %.2f euros \n", price);
			percentage = percentageFull(head->Next);
			printf("This bus is %.2f percent occupied!\n", percentage); //samo testiranje za mozda koristenje posli u admin-view i dizanju cijena
			if (percentageFull(head->Next) > 50) {
				printf("Price is increased due to high demand to %.2f \n", AdjustedPrice(percentage,price) );
			}
			PrintSeat(head);
			printf("Enter the amount of tickets you would like to buy: ");
			scanf("%d", &numberofTickets);
			for (int i = 0; i < numberofTickets;) {
				printf("\nChoose your seat: \n");
				scanf(" %s", SeatChoice);
				if (TakeSeat(head, SeatChoice, loggedIn) == EXIT_SUCCESS) { //ako je doslo do promjene rewrite inace nista 
					RewriteFile(head, selected->HourFilename, price);
					i++;
				}
			}
			system("cls");
			printf("New status: \n");//samo provjera da je dobro zauzeto 
			PrintSeat(head);
			printf("Input exit to quit, input anything else to proceed...\n");
			scanf(" %s",proceed);
			if (strcmp(proceed,"exit")==0) {
				break;
			}
			FreeList(head);
			FreeHourList(hourHead);
			puts("\n");
		}
		else {
			printf("Wrong input!\n");
		}
	} while (choice != 0);
	return EXIT_SUCCESS;
}

int RewriteFile(SeatPosition head, char* filename,float price) {
	FILE* fp = NULL;
	fp = fopen(filename, "w+");
	int counter = 0;
	if (NULL == fp) {
		perror("Error opening file!");
		return -1;
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

int TakeSeat(SeatPosition head, char* SeatChoice,Position loggedIn) {
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
	printf("That seat is not available!\n");
	return -2;
}