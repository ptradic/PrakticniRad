
#define _CRT_SECURE_NO_WARNINGS

#include "LogIn.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int ReservationMenu(Position loggedIn) {

	char BusArray[NUMBER_OF_ROUTES][10] = { 0 };
	char filetemp[NUMBER_OF_ROUTES][10] = { 0 };
	char timeselect[10] = { 0 };
	char SeatChoice[5] = { 0 };
	char proceed[10] = { 0 };
	
	float percentage = 0;
	float price = 0;

	int numberofTickets = 0;
	int choice = 0;
	int i = 0;

	HourList Head = { .HourFilename = "",.startingHour = "",.Next = NULL };
	Seat SeatHead = { .SeatName = "",.SeatState = "" ,.Next = NULL };
	SeatPosition head = &SeatHead;
	HourPosition hourHead = &Head;
	HourPosition selected;

	FILE* fp = NULL;
	fp = fopen("autobusi.txt", "r");

	if (!fp) {
		perror("Error opening bus file!");
		return ERROR;
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
			printf("%d %s \n", i + 1, BusArray[i]);
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

			do {
				printf("Enter the time at which you would like to travel: ");
				scanf(" %s", timeselect);

			} while ((selected = FindHours(hourHead, timeselect)) == NULL);
			//printf(" %s", selected->HourFilename);
			Insert(head, selected->HourFilename, &price); //napravi listu 
			printf("The price of a ticket on this line is: %.2f euros \n", price);
			percentage = percentageFull(head->Next);
			printf("This bus is %.2f percent occupied!\n", percentage); //samo testiranje za mozda koristenje posli u admin-view i dizanju cijena

			if (percentageFull(head->Next) > 50) {
				printf("Price is increased due to high demand to %.2f \n", AdjustedPrice(percentage, price));
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
			scanf(" %s", proceed);

			if (strcmp(proceed, "exit") == 0) {
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

int LoadAccountsFromFile(Position head) { //printfovi u whileu samo da se trenutno lakse prati sta se upisuje 
	FILE* fp = NULL;
	fp = fopen("racuni.txt", "r");
	if (!fp) {
		perror("Error opening account file!");
		return -1;
	}
	while (!feof(fp)) {
		Position newElement = NULL;
		newElement = AllocateMemory(newElement);
		fscanf_s(fp, "%s", newElement->username, MAX_SIZE);
		printf("%s ", newElement->username);
		fscanf_s(fp, "%s", newElement->password, MAX_SIZE);
		printf("%s ", newElement->password);
		if (strlen(newElement->username) > 0) { //ovi if-else samo provjerava da je ustvari skupia zbog praznih redova ako nije free ovo sta smo alocirali gore
			newElement = SortedInsert(head, newElement);
			printf("\n");
		}
		else {
			free(newElement);
		}
	}
	fclose(fp);
	return EXIT_SUCCESS;
}

int checkUsernameSize(char* username,int size) {
	while (strlen(username) > size) {
		printf("Wrong input maximum size is 20!\n");
		return -1;
	}
	return EXIT_SUCCESS;
}

int AddAccountToFile(Position head) { //dodavanje novog racuna u file i stablo s provjeravama zauzetosti maila i usernamea 
	char username[MAX_SIZE] = { 0 };
	char password[MAX_SIZE] = { 0 };
	int status = 0;
	int availability = 0;
	FILE* fp = NULL;
	fp = fopen("racuni.txt", "a");
	do {
		printf("Enter your username: ");
		scanf_s(" %s", username, MAX_SIZE);
		status = checkUsernameSize(username, 20);
		availability = FindElementByUser(head, username);
	} while (status != 0 || availability != 0);
	printf("Enter your password: ");
	scanf_s(" %s", password, MAX_SIZE);
	AddNewAccountToList(head, username, password);
	fprintf(fp, "%s", username);
	fprintf(fp, " %s\n", password);
	fclose(fp);
	return EXIT_SUCCESS;
}

Position AllocateMemory(Position current) {
	current = (Position)malloc(sizeof(BinarySearchTree));
	if (!current) {
		perror("Error Allocating memory! ");
		return NULL;
	}
	current->left = NULL;
	current->right = NULL;
	strcpy(current->password, "");
	strcpy(current->username, "");
	return current;
}

int FindElementByUser(Position current, char* username) { //provjerava jel zauzet username kojim pokusajemo registrirat 
	if (NULL == current)
	{
		return EXIT_SUCCESS;
	}
	if (strcmp(current->username, username) == 0)
	{
		printf("That username is already taken,try another one!\n");
		return -1;
	}
	else if (strcmp(current->username, username) < 0)
		FindElementByUser(current->right, username);

	else if (strcmp(current->username, username) > 0)
		FindElementByUser(current->left, username);
}

Position LogIn(Position head) { //pronalazi user, ako nade password tog usera usporeduje s nasim unosom ako je dobar uspjesno smo logirani
	char username[MAX_SIZE] = { 0 };
	char password[MAX_SIZE] = { 0 };
	char choice = '0';
	Position find = NULL;
	do {
		system("cls");
		printf("Input your login info: \n");
		printf("Username: ");
		scanf_s(" %s", username, MAX_SIZE);
		find = FindLoginUser(head, username);
		if (find == NULL) {
			printf("No such username found!\n");
			printf("Press 0 if you would like to exit login menu.\n"
				"Press any other button if you would like to try again.\n ");
			scanf_s(" %c", &choice, MAX_SIZE);
		}
		else {
			printf("Password: ");
			scanf_s(" %s", password, MAX_SIZE);
			if (strcmp(find->password, password) == 0) {
				printf("Login succsesful! \n");
				printf("Press any button to proceed...\n");
				getch();
				return find;
			}
			else {
				printf("Wrong password!\n");
				printf("Press 0 if you would like to exit login menu.\n"
					"Press any other button if you would like to try again.\n ");
				scanf_s(" %c", &choice, MAX_SIZE);
			}
		}
	} while (choice != '0');
	return NULL;
}

Position SortedInsert(Position current, Position newElement) { //sortirani unos po usernameu 
	if (current == NULL)
		return newElement;

	else if (strcmp(current->username, newElement->username) < 0)
		current->right = SortedInsert(current->right, newElement);

	else if (strcmp(current->username, newElement->username) > 0)
		current->left = SortedInsert(current->left, newElement);

	else
		free(newElement);

	return current;
}

int INorderPrint(Position current) { // print da se vidi jel normalno sortirano 
	if (current == NULL) {
		return 0;
	}

	INorderPrint(current->left);
	printf("%s \n", current->username);
	INorderPrint(current->right);
	return EXIT_SUCCESS;
}

int AddNewAccountToList(Position head, char* username, char* password) {
	Position newElement = NULL;
	newElement = AllocateMemory(newElement);
	strcpy(newElement->username, username);
	strcpy(newElement->password, password);
	newElement = SortedInsert(head, newElement);
	return EXIT_SUCCESS;
}

//pronalazi user i vraca current ako nade account u koji se pokusaje log inat,prakticki isto ka gornji finduser samo vraca position pa se mozda da to pojednostavnit
Position FindLoginUser(Position current, char* username) {
	if (NULL == current)
	{
		return NULL;
	}
	if (strcmp(current->username, username) == 0)
	{
		return current;
	}
	else if (strcmp(current->username, username) < 0)
		FindLoginUser(current->right, username);

	else if (strcmp(current->username, username) > 0)
		FindLoginUser(current->left, username);
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
	fp = fopen("autobusi.txt", "r");
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
			printf("Hour %s is at-> %.2f%% capacity, current price ->%.2f (starting price was %.2f)\n", temp->Next->startingHour, percentage, AdjustedPrice(percentage, price), price);
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

int DeleteAll(Position current) {
	if (current == NULL) {
		return 0;
	}
	DeleteAll(current->left);
	DeleteAll(current->right);
	free(current);
	return EXIT_SUCCESS;
}