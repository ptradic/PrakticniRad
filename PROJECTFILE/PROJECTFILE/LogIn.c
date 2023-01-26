#define _CRT_SECURE_NO_WARNINGS

#include "LogIn.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int LoadAccountsFromFile(Position head) {

	FILE* fp = NULL;
	fp = fopen("racuni.txt", "r");

	if (!fp) {
		perror("Error opening account file!");
		return PROGRAM_ERROR;
	}

	while (!feof(fp)) {

		Position newElement = NULL;
		newElement = AllocateMemory(newElement);

		fscanf_s(fp, "%s", newElement->username, MAX_SIZE);
		printf("%s ", newElement->username);
		fscanf_s(fp, "%s", newElement->password, MAX_SIZE);
		printf("%s ", newElement->password);

		if (strlen(newElement->username) > 0) {
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
		return PROGRAM_ERROR;
	}

	return EXIT_SUCCESS;
}

int AddAccountToFile(Position head) {

	char username[MAX_SIZE] = { 0 };
	char password[MAX_SIZE] = { 0 };
	int availability = 0;
	int status = 0;

	FILE* fp = NULL;
	fp = fopen("racuni.txt", "a");

	do {
		printf("Enter your username: ");
		scanf(" %s", username);

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

int FindElementByUser(Position current, char* username) {

	if (NULL == current)
	{
		return EXIT_SUCCESS;
	}

	if (strcmp(current->username, username) == 0)
	{
		printf("That username is already taken,try another one!\n");
		return PROGRAM_ERROR;
	}

	else if (strcmp(current->username, username) < 0)
		FindElementByUser(current->right, username);

	else if (strcmp(current->username, username) > 0)
		FindElementByUser(current->left, username);
}

Position LogIn(Position head) {

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

Position SortedInsert(Position current, Position newElement) {
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

int INorderPrint(Position current) {

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

int DeleteAll(Position current) {

	if (current == NULL) {
		return 0;
	}

	DeleteAll(current->left);
	DeleteAll(current->right);
	free(current);

	return EXIT_SUCCESS;
}