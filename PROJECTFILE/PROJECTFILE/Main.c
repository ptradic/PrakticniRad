#define _CRT_SECURE_NO_WARNINGS

#include "LogIn.h"
#include "Seat.h"
#include "HourList.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
	
//#define MAX_LINE 1024

float AdjustedPrice(float percentageFull, float price);
int TakeSeat(SeatPosition head, char* SeatChoice, Position loggedIn);

int main() {

	BinarySearchTree headAccount = { .username = " ",.password = "",.right = NULL,.left = NULL };
	Position head = &headAccount;
	Position loggedIn = NULL;

	LoadAccountsFromFile(head); //dodaje u listu racune iz filea 
	//INorderPrint(head); //samo da se lakse prate trenutni racuni 

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
				//printf("You're Logged into an account with following credentials:\n");
				//printf("%s %s\n", loggedIn->username, loggedIn->password);
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