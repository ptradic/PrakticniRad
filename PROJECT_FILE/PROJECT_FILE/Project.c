#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#define MAX_LINE 1024
#define MAX_SIZE 128

typedef struct _BinarySearchTree* Position;
typedef struct _BinarySearchTree {
	char username[MAX_SIZE];
	char email[MAX_SIZE];
	char password[MAX_SIZE];
	Position left;
	Position right;
}BinarySearchTree;

int LoadAccountsFromFile(Position head);
int AddAccountToFile(Position head);
Position AllocateMemory(Position current);
int FindElementByUser(Position current, char* username);
int FindElementMail(Position current, char* email);
Position LogIn(Position head);
Position SortedInsert(Position current, Position newElement);
int INorderPrint(Position current);
//int DeleteAll(Position current);

int main() {
	BinarySearchTree headAccount = { .username = "",.email = "",.password = "",.right = NULL,.left=NULL };
	Position head = &headAccount;
	Position loggedIn = NULL;
	LoadAccountsFromFile(head); //dodaje u listu racune iz filea 
	INorderPrint(head); //samo da se lakse prate trenutni racuni 
	char choice = '0';
	do {
		printf("\n1-Register a new account\n2-Login\n0-Exit\n ");
		scanf(" %c", &choice);
		switch (choice) {
		case '1':
			AddAccountToFile(head);
			break;
		case '2':
			loggedIn = LogIn(head);
			if (loggedIn != NULL) {
				printf("You're Logged into an account with following credentials:\n");
				printf("%s %s %s\n", loggedIn->username, loggedIn->email, loggedIn->password);
				//RESERVATION_MENU();
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
int LoadAccountsFromFile(Position head) { //printfovi u whileu samo da se trenutno lakse prati sta se upisuje 
	char buffer[MAX_LINE] = { 0 };
	FILE* fp = NULL;
	fp = fopen("racuni.txt", "r");
	if (!fp) {
		perror("Error opening file!");
		return -1;
	}
	while (!feof(fp)) {
		Position newElement = NULL;
		newElement = AllocateMemory(newElement);
		fscanf(fp, "%s", newElement->username);
		printf("%s ", newElement->username);
		fscanf(fp, "%s", newElement->email);
		printf("%s ", newElement->email);
		fscanf(fp, "%s", newElement->password);
		printf("%s ", newElement->password);
		if (strlen(newElement->username) > 0) { //ovi if-else samo provjerava da je ustvari skupia zbog praznih redova ako nije free ovo sta smo alocirali gore
			newElement=SortedInsert(head, newElement);
			printf("\n");
		}
		else {
			free(newElement);
		}
	}
	fclose(fp);
	return EXIT_SUCCESS;
}
Position SortedInsert(Position current, Position newElement) { //sortirani unos po usernameu 
	if (current == NULL)
		return newElement;

	else if (strcmp(current->username,newElement->username)<0)
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
Position AllocateMemory(Position current) {
	current = (Position)malloc(sizeof(BinarySearchTree));
	if (!current) {
		perror("Error Allocating memory! ");
		return NULL;
	}
	strcpy(current->email, "");
	current->left = NULL;
	current->right = NULL;
	strcpy(current->password, "");
	strcpy(current->username, "");
	return current;
}
int AddAccountToFile(Position head) { //dodavanje novog racuna u file i stablo s provjeravama zauzetosti maila i usernamea 
	char username[MAX_SIZE] = { 0 };
	char email[MAX_SIZE] = { 0 };
	char password[MAX_SIZE] = { 0 };
	int status = 0;
	FILE* fp = NULL;
	fp = fopen("racuni.txt", "a");
	printf("Unesi korisnicko ime: ");
	scanf(" %s", username);
	status = FindElementByUser(head, username);
	while (status != 0) {
		printf("Ponovi unos korisnickog imena: ");
		scanf(" %s", username);
		status = FindElementByUser(head, username);
	}
	printf("\nUnesi email adresu: ");
	scanf(" %s", email);
	status = FindElementMail(head, email);
	while (status != 0) {
		printf("Ponovi unos email adrese: ");
		scanf(" %s", email);
		status = FindElementMail(head, email);
	}
	printf("Unesi lozinku ime: ");
	scanf(" %s", password);
	AddNewAccountToList(head, username, email, password);
	fprintf(fp, " %s", username);
	fprintf(fp, " %s", email);
	fprintf(fp, " %s\n", password);
	fclose(fp);
	return EXIT_SUCCESS;
}
int FindElementByUser(Position current,char* username) { //provjerava jel zauzet username kojim pokusajemo registrirat 
	if (NULL == current)
	{
		return EXIT_SUCCESS;
	}
	if (strcmp(current->username,username)==0)
	{
		printf("username already taken!\n");
		return -1;
	}
	else if (strcmp(current->username,username)<0)
		FindElementByUser(current->right, username);

	else if (strcmp(current->username, username) > 0)
		FindElementByUser(current->left, username);
}
int FindElementMail(Position current, char* email) { //provjerava jel zauzet mail kojim pokusajemo registrirat 
	if (NULL == current)
	{
		return EXIT_SUCCESS;
	}
	if (strcmp(current->email, email) == 0)
	{
		printf("email already in use!\n");
		return -1;
	}
	else if (strcmp(current->email, email) < 0)
		FindElementMail(current->right, email);

	else if (strcmp(current->email, email) > 0)
		FindElementMail(current->left, email);
}
int AddNewAccountToList(Position head, char* username, char* email, char* password) {
	Position newElement = NULL;
	newElement = AllocateMemory(newElement);
	strcpy(newElement->email, email);
	strcpy(newElement->username, username);
	strcpy(newElement->password, password);
	newElement=SortedInsert(head, newElement);
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
Position LogIn(Position head) { //pronalazi user, ako nade password tog usera usporeduje s nasim unosom ako je dobar uspjesno smo logirani
	char username[MAX_SIZE] = { 0 };
	char password[MAX_SIZE] = { 0 };
	char choice = '0';
	Position find = NULL;
	do {
		printf("Input your login info: \n");
		printf("Username: ");
		scanf(" %s", username);
		find = FindLoginUser(head, username);
		if (find == NULL) {
			printf("No such username found!\n");
			printf("Press 0 if you would like to exit login menu.\n"
				"Press any other button if you would like to try again.\n ");
			scanf(" %c", &choice);
		}
		else {
			printf("Password: ");
			scanf(" %s", password);
			if (strcmp(find->password, password) == 0) {
				printf("Login succsesful! \n");
				return find;
			}
			else {
				printf("Wrong password!\n");
				printf("Press 0 if you would like to exit login menu.\n"
					"Press any other button if you would like to try again.\n ");
				scanf(" %c", &choice);
			}
		}
	} while (choice != '0');
	return NULL;
}
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