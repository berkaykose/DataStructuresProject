#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<assert.h>
#include<time.h>
#include <stdbool.h>

#define SIZE1 1000
#define SIZE2 10000
#define SIZE3 100000
#define SIZE4 1000000

typedef struct password {
	char password[30];
} Pass;

typedef struct node {
	int indexId;
	char password[30];
	struct node* next;
} Node;

Node *head = NULL;

Node * createNewNode(int index, Pass passList[], int size) {
	Node *result = malloc(sizeof(Node));
	result->indexId = size - index - 1;
	strcpy(result->password, passList[size - index - 1].password);
	result->next = NULL;
	return result;
}

Node *search (Node *head, int key) {
	Node *tmp = head;
	
	while (tmp) {
		if (tmp->indexId == key) {
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}

void printList(Node *head) {
	Node *temporary = head;
	while (temporary != NULL) {
		printf("%d --- %s \n", temporary->indexId, temporary->password);
		temporary = temporary->next;
	}
}

unsigned int hash (char* password, int tableSize) {
	int length = strnlen(password, 30);
	int i;
	unsigned int hashValue = 0;
	for (i = 0; i < length; i++) {
		hashValue += password[i];
		hashValue = (hashValue * password[i]) % tableSize;
	}
	return hashValue;
}

void initHashTable (Pass* hashTable[], int tableSize) {
	int i;
	for (i = 0; i < tableSize; i++)
		hashTable[i] = NULL;
}

int hashTableInsert(Pass* hashTable[], Pass *p, int tableSize, int* collCount) {
	if (p == NULL) return -1;
	int index = hash(p->password, tableSize), i;
	for (i = 0; i < tableSize; i++) {
		int try = (i + index) % tableSize;
		if (hashTable[try] == NULL) {
			hashTable[try] = p;
			return 0;;
		} else {
			(*collCount)++;
		}
	}
	return -1;
}

void merge(Pass passList[], int left, int middle, int right, int state) {
    
    //printf("%d\n", middle);
	int leftSize = middle - left + 1;
	int rightSize = right - middle;
	
	//printf("%d\n", rightSize);
	//printf("%d\n", rightSize);
	//Pass leftArr[leftSize];
	Pass *leftArr = (Pass *)malloc(leftSize * sizeof(Pass));
    //Pass rightArr[rightSize];
    Pass *rightArr = (Pass *)malloc(rightSize * sizeof(Pass));
	
	//printf("%s -- %s \n", passList[0], rightArr[0]);
	int i, j;
	for (i = 0; i < leftSize; i++) {
		strcpy(leftArr[i].password, passList[left + i].password);
	}
	
	for (j = 0; j < rightSize; j++) {
		strcpy(rightArr[j].password, passList[middle + 1 + j].password);
	}
	
	
	i = 0; j = 0; int org = left;
	while (i < leftSize && j < rightSize) {
		if (state == 1) {
			if (strcmp(leftArr[i].password, rightArr[j].password) <= 0) {
				strcpy(passList[org].password, leftArr[i].password);
				i++;
			} else {
				strcpy(passList[org].password, rightArr[j].password);
				j++;
			}
			org++;
		} else {
			if (strcmp(leftArr[i].password, rightArr[j].password) > 0) {
				strcpy(passList[org].password, leftArr[i].password);
				i++;
			} else {
				strcpy(passList[org].password, rightArr[j].password);
				j++;
			}
			org++;
		}
		
	}
	
	while (i < leftSize) {
		strcpy(passList[org].password, leftArr[i].password);
		org++;
		i++;
	}
	while (j < rightSize) {
		strcpy(passList[org].password, rightArr[j].password);
		org++;
		j++;
	}
	free(leftArr);
	free(rightArr);
}

void mergeSort(Pass passList[], int left, int right, int state) {
	//printf("%d\n", right);
    if (left < right) {
    	int middle = (left + right) / 2;
    	mergeSort(passList, left, middle, state);
    	mergeSort(passList, middle + 1, right, state);
    	
    	merge(passList, left, middle, right, state);
	}
}


int linearSearch (Pass passList[], int size, char* key) {
	int temp = -1;
	int i;
	for (i = 0; i < size; i++) {
		if(strcmp(passList[i].password, key) == 0)
			temp = i;
	}
	
	return temp;
}

int main()
{
	/* code */
	int choose;
	printf("Choose the data source --> \n");
	printf("1-1.000 Password List \n");
	printf("2-10.000 Password List \n");
	printf("3-100.000 Password List \n");
	printf("4-1.000.000 Password List \n");
	printf("Your choise --> ");
	scanf("%d", &choose);
	
	switch(choose) {
		case 1: {
			char takenPass[30];
			Pass passwords[SIZE1];
			Pass* hashTable[SIZE1];
			initHashTable(hashTable, SIZE1);
			
			Pass* searchInTable (char* key) {
				int index = hash(key, SIZE1), i;
				for (i = 0; i < SIZE1; i++) {
					int try = (index + i) % SIZE1;
					if (hashTable[try] != NULL && strcmp(hashTable[try]->password, key) == 0) {
						return hashTable[try];
					}
				}
				return NULL;
			}
			
			
			int i;	
			FILE *fp = fopen("10-million-password-list-top/1000.txt", "r");
			
			for(i = 0; i < SIZE1; i++) {
				fscanf(fp, "%s", &passwords[i].password);
			}
			
			fclose(fp);
			
			int obj;
			printf("\nChoose the Objective --> \n");
			printf("1-Search for any given password \n");
			printf("2-Sort the list asc and desc order \n");
			printf("3-Calculate appropriate hash table size \n");
			printf("4-Create a singly linked list node structure \n");
			printf("Your choise --> ");
			scanf("%d", &obj);
			
			switch(obj) {
				case 1: {
					printf("Enter a password to be searched: \n");
					scanf("%s", &takenPass);
					
					int res = linearSearch(passwords, SIZE1, takenPass);

					if (res == -1) {
						printf("Given password can`t found");
					} else {
						printf("Password found, position of it: %d", res);
					}

					break;
				}
				case 2: {
					mergeSort(passwords, 0, SIZE1 - 1, 1);
					printf("Asc order: ");
					int i;
					for (i = 0; i < 8; i++) {
						printf("%s\t", passwords[i].password);
					}
					
					printf("\n");
					printf("Desc order: ");
					mergeSort(passwords, 0, SIZE1 - 1, 0);
					
					for (i = 0; i < 8; i++) {
						printf("%s\t", passwords[i].password);
					}
					
					break;
				}
				case 3: {
					printf("Enter a password to be searched: \n");
					scanf("%s", &takenPass);
					
					int i, temp, collisionCounter = 0;
					for (i = 0; i < SIZE1; i++) {
						hashTableInsert(hashTable, &passwords[i], SIZE1, &collisionCounter);
					}
					for (i = 0; i < SIZE1; i++) {
						if (strcmp(hashTable[i], takenPass) == 0)	
							temp = i;
					}
					
					Pass* tmp = searchInTable(takenPass);
					collisionCounter = collisionCounter % SIZE1;
					if(tmp == NULL)
						printf("Not found\n");
					else {
						printf("Password found in the index of %d\n",temp);
						printf("Number of collision: %d", collisionCounter);
					}
						
					
					break;
				}
				case 4: {
					Node *head = NULL;
					Node *tmp;
					int key = 0;
					clock_t start_t, end_t, total_t;
					
					printf("Enter a index to be searched: \n");
					scanf("%d", &key);
					
					int i;
					for (i = 0; i < SIZE1; i++) {
						tmp = createNewNode(i, passwords, SIZE1);
						tmp->next = head;
						head = tmp;
					}
					
					//printList(head);
					
					start_t = clock();
					tmp = search(head, key);
					end_t = clock();
					total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
					printf("Found node with index: %d and password: %s in %f seconds", tmp->indexId, tmp->password, total_t);
					break;
				}
				default: printf("\nWrong choise");
			}
			
			break;
		}
		case 2: {
			char takenPass[30];
			Pass passwords[SIZE2];
			Pass* hashTable[SIZE2];
			initHashTable(hashTable, SIZE2);
			
			Pass* searchInTable (char* key) {
				int index = hash(key, SIZE2), i;
				for (i = 0; i < SIZE2; i++) {
					int try = (index + i) % SIZE2;
					if (hashTable[try] != NULL && strcmp(hashTable[try]->password, key) == 0) {
						return hashTable[try];
					}
				}
				return NULL;
			}
			
			int i;
			FILE *fp = fopen("10-million-password-list-top/10000.txt", "r");
			
			for(i = 0; i < SIZE2; i++) {
				fscanf(fp, "%s", &passwords[i].password);
			}
			
			fclose(fp);
			
			int obj;
			printf("\nChoose the Objective --> \n");
			printf("1-Search for any given password \n");
			printf("2-Sort the list asc and desc order \n");
			printf("3-Calculate appropriate hash table size \n");
			printf("4-Create a singly linked list node structure \n");
			printf("Your choise --> ");
			scanf("%d", &obj);
			
			switch(obj) {
				case 1: {
					printf("Enter a password to be searched: \n");
					scanf("%s", &takenPass);
					
					int res = linearSearch(passwords, SIZE2, takenPass);

					if (res == -1) {
						printf("Given password can`t found");
					} else {
						printf("Password found, position of it: %d", res);
					}

					break;
				}
				case 2: {
					mergeSort(passwords, 0, SIZE2 - 1, 1);
					
					printf("Asc order: ");
					int i;
					for (i = 0; i < 8; i++) {
						printf("%s\t", passwords[i].password);
					}
					
					printf("\n");
					printf("Desc order: ");
					mergeSort(passwords, 0, SIZE2 - 1, 0);
					
					for (i = 0; i < 8; i++) {
						printf("%s\t", passwords[i].password);
					}
					
					break;
				}
				case 3: {
					printf("Enter a password to be searched: \n");
					scanf("%s", &takenPass);
					
					int i, temp, collisionCounter = 0;
					
					for (i = 0; i < SIZE2; i++) {
						hashTableInsert(hashTable, &passwords[i], SIZE2, &collisionCounter);
					}
					for (i = 0; i < SIZE2; i++) {
						if (strcmp(hashTable[i], takenPass) == 0)	
							temp = i;
					}
					collisionCounter = collisionCounter % SIZE2;
					Pass* tmp = searchInTable(takenPass);
					if(tmp == NULL)
						printf("Not found\n");
					else {
						printf("Password found in the index of %d\n",temp);
						printf("Number of collision: %d", collisionCounter);
					}
						
					
					break;
				}
				case 4: {
					Node *head = NULL;
					Node *tmp;
					int key = 0;
					clock_t start_t, end_t, total_t;
					
					printf("Enter a index to be searched: \n");
					scanf("%d", &key);
					
					int i;
					for (i = 0; i < SIZE2; i++) {
						tmp = createNewNode(i, passwords, SIZE2);
						tmp->next = head;
						head = tmp;
					}
			
					//printList(head);
					
					
					start_t = clock();
					tmp = search(head, key);
					end_t = clock();
					total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
					printf("Found node with index: %d and password: %s in %f seconds", tmp->indexId, tmp->password, total_t);
					
					break;
				}
				default: printf("\nWrong choise");
			}
			
			break;
		}
		case 3: {
			char takenPass[30];
			static Pass passwords[SIZE3];
			static Pass* hashTable[SIZE3];
			initHashTable(hashTable, SIZE3);
			
			Pass* searchInTable (char* key) {
				int index = hash(key, SIZE3), i;
				for (i = 0; i < SIZE3; i++) {
					int try = (index + i) % SIZE3;
					if (hashTable[try] != NULL && strcmp(hashTable[try]->password, key) == 0) {
						return hashTable[try];
					}
				}
				return NULL;
			}
			
			int i;
			FILE *fp = fopen("10-million-password-list-top/100000.txt", "r");
			
			for(i = 0; i < SIZE3; i++) {
				fscanf(fp, "%s", &passwords[i].password);
			}
			
			fclose(fp);
			
			int obj;
			printf("\nChoose the Objective --> \n");
			printf("1-Search for any given password \n");
			printf("2-Sort the list asc and desc order \n");
			printf("3-Calculate appropriate hash table size \n");
			printf("4-Create a singly linked list node structure \n");
			printf("Your choise --> ");
			scanf("%d", &obj);
			
			switch(obj) {
				case 1: {
					printf("Enter a password to be searched: \n");
					scanf("%s", &takenPass);
					
					//printf("%s", passwords[9999].password);
					int res = linearSearch(passwords, SIZE3, takenPass);
					
					
					if (res == -1) {
						printf("Given password can`t found");
					} else {
						printf("Password found, position of it: %d", res);
					}

					break;
				}
				case 2: {
					mergeSort(passwords, 0, SIZE3 - 1, 1);
					printf("Asc order: ");
					int i;
					for (i = 0; i < 8; i++) {
						printf("%s\t", passwords[i].password);
					}
					
					printf("\n");
					printf("Desc order: ");
					mergeSort(passwords, 0, SIZE3 - 1, 0);
					
					for (i = 0; i < 8; i++) {
						printf("%s\t", passwords[i].password);
					}
					
					break;
				}
				case 3: {
					printf("Enter a password to be searched: \n");
					scanf("%s", &takenPass);
					
					int i, temp, collisionCounter = 0;
					for (i = 0; i < SIZE3; i++) {
						hashTableInsert(hashTable, &passwords[i], SIZE3, &collisionCounter);
					}
					for (i = 0; i < SIZE3; i++) {
						if (strcmp(hashTable[i], takenPass) == 0)	
							temp = i;
					}
					collisionCounter = collisionCounter % SIZE3;
					Pass* tmp = searchInTable(takenPass);
					if(tmp == NULL)
						printf("Not found\n");
					else {
						printf("Password found in the index of %d\n",temp);
						printf("Number of collision: %d", collisionCounter);
					}
					
					break;
				}
				case 4: {
					Node *head = NULL;
					Node *tmp;
					int key = 0;
					clock_t start_t, end_t, total_t;
					
					printf("Enter a index to be searched: \n");
					scanf("%d", &key);

					int i;
					for (i = 0; i < SIZE3; i++) {
						tmp = createNewNode(i, passwords, SIZE3);
						tmp->next = head;
						head = tmp;
					}
			
					//printList(head);
					
					
					start_t = clock();
					tmp = search(head, key);
					end_t = clock();
					total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
					printf("Found node with index: %d and password: %s in %f seconds", tmp->indexId, tmp->password, total_t);
					
					break;	
				}
				default: printf("\nWrong choise");
			}
			
			break;
		}
		case 4: {
			char takenPass[30];
			static Pass passwords[SIZE4];
			static Pass* hashTable[SIZE4];
			initHashTable(hashTable, SIZE4);
			
			Pass* searchInTable (char* key) {
				int index = hash(key, SIZE4), i;
				for (i = 0; i < SIZE4; i++) {
					int try = (index + i) % SIZE4;
					if (hashTable[try] != NULL && strcmp(hashTable[try]->password, key) == 0) {
						return hashTable[try];
					}
				}
				return NULL;
			}
			
			int i;
			FILE *fp = fopen("10-million-password-list-top/1000000.txt", "r");
			
			for(i = 0; i < SIZE4; i++) {
				fscanf(fp, "%s", &passwords[i].password);
			}
			
			fclose(fp);
			
			int obj;
			printf("\nChoose the Objective --> \n");
			printf("1-Search for any given password \n");
			printf("2-Sort the list asc and desc order \n");
			printf("3-Calculate appropriate hash table size \n");
			printf("4-Create a singly linked list node structure \n");
			printf("Your choise --> ");
			scanf("%d", &obj);
			
			switch(obj) {
				case 1: {
					printf("Enter a password to be searched: \n");
					scanf("%s", &takenPass);
					
					int res = linearSearch(passwords, SIZE4, takenPass);
					
					
					if (res == -1) {
						printf("Given password can`t found");
					} else {
						printf("Password found, position of it: %d", res);
					}

					break;
				}
				case 2: {
					mergeSort(passwords, 0, SIZE4 - 1, 1);
					
					printf("Asc order: ");
					int i;
					for (i = 0; i < 8; i++) {
						printf("%s\t", passwords[i].password);
					}
					
					printf("\n");
					printf("Desc order: ");
					mergeSort(passwords, 0, SIZE4 - 1, 0);
					
					for (i = 0; i < 8; i++) {
						printf("%s\t", passwords[i].password);
					}
					
					break;
				}
				case 3: {
					printf("Enter a password to be searched: \n");
					scanf("%s", &takenPass);
					
					int i, temp, collisionCounter = 0;
					for (i = 0; i < SIZE4; i++) {
						hashTableInsert(hashTable, &passwords[i], SIZE4, &collisionCounter);
					}
					for (i = 0; i < SIZE4; i++) {
						if (strcmp(hashTable[i], takenPass) == 0)	
							temp = i;
					}
					collisionCounter = collisionCounter % SIZE4;
					Pass* tmp = searchInTable(takenPass);
					if(tmp == NULL)
						printf("Not found\n");
					else {
						printf("Password found in the index of %d\n",temp);
						printf("Number of collision: %d", collisionCounter);
					}
					
					break;
				}
				case 4: {
					Node *head = NULL;
					Node *tmp;
					int key = 0;
					clock_t start_t, end_t, total_t;
					
					printf("Enter a index to be searched: \n");
					scanf("%d", &key);
					
					int i;
					for (i = 0; i < SIZE4; i++) {
						tmp = createNewNode(i, passwords, SIZE4);
						tmp->next = head;
						head = tmp;
					}
			
					//printList(head);
					
					
					start_t = clock();
					tmp = search(head, key);
					end_t = clock();
					total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
					printf("Found node with index: %d and password: %s in %f seconds", tmp->indexId, tmp->password, total_t);
					
					break;
				}
				default: printf("\nWrong choise");
			}
			
			break;
		}
		default: printf("Wrong choise");
		
	}
	
	
	return 0;
}




	
