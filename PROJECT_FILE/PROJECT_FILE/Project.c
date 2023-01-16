#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#define MAX_LINE 1024
#define MAX_SIZE 128
#define NUMBER_OF_ROUTES 6	

typedef struct _BinarySearchTree* Position;
typedef struct _BinarySearchTree {
	char username[MAX_SIZE];
	char password[MAX_SIZE];
	Position left;
	Position right;
}BinarySearchTree;


//racuna mozda umisto nizova stavljat ovako u listu al nije toliko prakticnije pa nez 
/*typedef struct _BusList* BusPosition;
typedef struct _BusList {
	char route[10];
	int price;
	char filename[10];
	BusPosition next;
}BusList;*/

int LoadAccountsFromFile(Position head);
int AddAccountToFile(Position head);
Position AllocateMemory(Position current);
int FindElementByUser(Position current, char* username);
Position LogIn(Position head);
Position SortedInsert(Position current, Position newElement);
int INorderPrint(Position current);
int ReservationMenu(Position loggedIn);
int SeatFile(char* filename);
//int DeleteAll(Position current);

int main() {
	BinarySearchTree headAccount = { .username = "",.password = "",.right = NULL,.left=NULL };
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
	int choice=0;
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
			printf("%d %s %s\n",i+1, BusArray[i], filetemp[i]); //samo za pracenje da je dobro zapisano
		}
		printf("Choose your route or press 0 to exit: \n");
		scanf(" %d", &choice);
		if (choice == 0) {
			printf("Exited to main menu!\n");
			break;
		}
		if (choice <= NUMBER_OF_ROUTES ) {
			SeatFile(filetemp[choice - 1]); //salje ime filea odabrane rute 
		}
		else {
			printf("Wrong input!\n");
		}
	} while (choice != 0);

	return EXIT_SUCCESS;
}
int SeatFile(char* filename) {
	FILE* fp = NULL;
	fp = fopen(filename, "r");
	char temp[20] = { 0 };
	int numberOfSeats = 0;
	fscanf(fp, " %d", &numberOfSeats);
	//sad array s statusom sjedalica napravit i onda iza toga odabir stolice, mijenjanje statusa u filu itd.
	while (!feof(fp)) {
		fscanf(fp, " %s",temp);
		printf(" %s", temp); //samo za provjeravanje da normalno cita 
	}
	puts("\n");
	fclose(fp);
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
		fscanf(fp, "%s", newElement->username);
		printf("%s ", newElement->username);
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
	current->left = NULL;
	current->right = NULL;
	strcpy(current->password, "");
	strcpy(current->username, "");
	return current;
}
int AddAccountToFile(Position head) { //dodavanje novog racuna u file i stablo s provjeravama zauzetosti maila i usernamea 
	char username[MAX_SIZE] = { 0 };
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
	printf("Unesi lozinku: ");
	scanf(" %s", password);
	AddNewAccountToList(head, username, password);
	fprintf(fp, "%s", username);
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

int AddNewAccountToList(Position head, char* username, char* password) {
	Position newElement = NULL;
	newElement = AllocateMemory(newElement);
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