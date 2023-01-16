#include "LogIn.h"
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

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

int AddAccountToFile(Position head) { //dodavanje novog racuna u file i stablo s provjeravama zauzetosti maila i usernamea 
	char username[MAX_SIZE] = { 0 };
	char password[MAX_SIZE] = { 0 };
	int status = 0;
	FILE* fp = NULL;
	fp = fopen("racuni.txt", "a");
	printf("Unesi korisnicko ime: ");
	scanf_s(" %s", username, MAX_SIZE);
	status = FindElementByUser(head, username);
	while (status != 0) {
		printf("Ponovi unos korisnickog imena: ");
		scanf_s(" %s", username, MAX_SIZE);
		status = FindElementByUser(head, username);
	}
	printf("Unesi lozinku: ");
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
		printf("username already taken!\n");
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