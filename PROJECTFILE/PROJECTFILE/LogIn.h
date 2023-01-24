#pragma once

#define MAX_SIZE 50

typedef struct _BinarySearchTree* Position;
typedef struct _BinarySearchTree {
	char username[MAX_SIZE];
	char password[MAX_SIZE];
	Position left;
	Position right;
}BinarySearchTree;

int checkUsernameSize(char* username, int size);
int LoadAccountsFromFile(Position head);
int AddAccountToFile(Position head);
Position AllocateMemory(Position current);
int FindElementByUser(Position current, char* username);
Position LogIn(Position head);
Position SortedInsert(Position current, Position newElement);
int INorderPrint(Position current);
int AddNewAccountToList(Position head, char* username, char* password);
Position FindLoginUser(Position current, char* username);
