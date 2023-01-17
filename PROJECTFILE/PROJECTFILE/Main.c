#define _CRT_SECURE_NO_WARNINGS

#include "LogIn.h"
#include "Seat.h"
#include<stdio.h>
#include<stdlib.h>
#define MAX_LINE 1024
#define NUMBER_OF_ROUTES 6	


//racuna mozda umisto nizova stavljat ovako u listu al nije toliko prakticnije pa nez 
/*typedef struct _BusList* BusPosition;
typedef struct _BusList {
	char route[10];
	int price;
	char filename[10];
	BusPosition next;
}BusList;*/

int ReservationMenu(Position loggedIn);
int SeatFile(char* filename);
//int DeleteAll(Position current);

int main() {

	BinarySearchTree headAccount = { .username = "",.password = "",.right = NULL,.left = NULL };
	Position head = &headAccount;
	Position loggedIn = NULL;
	LoadAccountsFromFile(head); //dodaje u listu racune iz filea 
	INorderPrint(head); //samo da se lakse prate trenutni racuni 
	char choice = '0';
	do {
		printf("\n1-Register a new account\n2-Login\n0-Exit\n ");
		scanf_s(" %c", &choice, MAX_SIZE);
		switch (choice) {
		case '1':
			AddAccountToFile(head);
			break;
		case '2':
			loggedIn = LogIn(head);
			if (loggedIn != NULL) {
				printf("You're Logged into an account with following credentials:\n");
				printf("%s %s\n", loggedIn->username, loggedIn->password);
				ReservationMenu(loggedIn);
			} //printfovi samo da se vidi da je na tocnom useru i sad ulazi u glavni meni di bi tribali rezervirat i ostalo 
			break;
		case '0':
			break;
		default:
			printf("Wrong input!");
			break;
		}
	} while (choice != '0');
	//DeleteAll(head); breaka program ovo triban jos vidit zasto
	return EXIT_SUCCESS;
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
	int counter = 0;
	char SeatChoice[5] = { 0 };
	SeatPosition temp = head;
	int success = 0;
	if (!fp) {
		perror("Error opening bus file!");
		return -1;
	}
	while (!feof(fp)) {
		fscanf(fp, "%s %s", BusArray[i], filetemp[i]);
		i++;
		//MakeList(temp, filetemp);
	}
	fclose(fp);
	do {
		for (int i = 0; i < NUMBER_OF_ROUTES; i++) {
			printf("%d %s %s\n", i + 1, BusArray[i], filetemp[i]); //samo za pracenje da je dobro zapisano
		}
		printf("Choose your route or press 0 to exit: \n");
		scanf(" %d", &choice);
		if (choice == 0) {
			printf("Exited to main menu!\n");
			break;
		}
		if (choice <= NUMBER_OF_ROUTES) {
			Insert(head, filetemp[choice - 1]); //napravi listu 
			while (temp->Next != NULL) { //isprinta sjedala 
				printf("%s %s ", temp->SeatName, temp->SeatState);
				temp = temp->Next;
				counter++;
				if (counter % 4 == 0) {
					printf("\n");
				}
			}
			printf("\nChoose your seat: \n");
			scanf(" %s", SeatChoice);
			if (TakeSeat(head, SeatChoice, loggedIn) == EXIT_SUCCESS) { //ako je doslo do promjene rewrite inace nista 
				RewriteFile(head, filetemp[choice-1]);
			}
			printf("\n New status: \n"); //samo provjera da je dobro zauzeto 
			while (head->Next != NULL) {
				printf("%s %s ", head->SeatName, head->SeatState);
				head = head->Next;
				counter++;
				if (counter % 4 == 0) {
					printf("\n");
				}
			}
			puts("\n");
		}
		else {
			printf("Wrong input!\n");
		}
	} while (choice != 0);
	return EXIT_SUCCESS;
}
int RewriteFile(SeatPosition head, char* filename) {
	FILE* fp = NULL;
	fp = fopen(filename, "w+");
	int counter = 0;
	if (NULL == fp) {
		perror("Error opening file!");
		return -1;
	}
	while (head->Next != NULL) {
		fprintf(fp, "%s %s ", head->SeatName, head->SeatState);
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
	while (head->Next != NULL) {
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

/*int SeatFile(char* filename) {
	FILE* fp = NULL;
	fp = fopen(filename, "r");
	char temp[20] = { 0 };
	int numberOfSeats = 0;
	fscanf(fp, " %d", &numberOfSeats);
	//sad array s statusom sjedalica napravit i onda iza toga odabir stolice, mijenjanje statusa u filu itd.
	while (!feof(fp)) {
		fscanf(fp, " %s", temp);
		printf(" %s", temp); //samo za provjeravanje da normalno cita 
	}
	puts("\n");
	fclose(fp);
	return EXIT_SUCCESS;
}*/

/*int DeleteAll(Position current) {
	if (current == NULL) {
		return 0;
	}
	DeleteAll(current->left);
	DeleteAll(current->right);
	printf("Deleting %s", current->username);
	free(current);
	return EXIT_SUCCESS;
}*/