/*
 * CValerio_thd.c
 *
 * TO COMPILE: gcc -o CValerio_thd.exe CValerio_thd.c -lpthread
 *
 * Author: Christopher Valerio
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

//unsigned long int sum; // data shared by the threads
long long sum; //variable used to sum child intervals
int runningPart3 = 0; // used to know if running_part3 is executing used for diff actions in functions it
int currChild = 0;
int currGChild = 0;


void *runner(void *param); // threads call this function to do final summation between given interval

void *runner2(void *param); // threads call this function to set up grand child threads

void run_part2(void* num, void* numThreads);
//int sumArr[];
//int childSum[];



struct Node{
	//unsigned long int data;
	long long data;
	int numGChild;
	struct Node *next;
};



struct Node* sumList;
//struct Node* tail;

void add(struct Node* head, unsigned long int newData, int newG) {
	//printf("Adding %d to sumList\n", newData);
	struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
	new_node->data  = newData;
	new_node->numGChild  = newG;
	new_node->next  = NULL;

	struct Node* curr = head;
	while(curr->next != NULL) {
		curr = curr->next;
	}
	curr->next = new_node;
}


void deleteList(struct Node* head) {

	struct Node* curr = head;
	struct Node* next;
	while(curr != NULL) {

		next = curr->next;
		free(curr);
		curr = next;
	}

}




void printList(struct Node* head) {
	long long i = 0;
	struct Node* curr = head->next;
	printf("Printing SumArr/List\n");
	while(curr != NULL) {
		printf("\t%d: GChild: %d -- Sum: '%llu' \n", i++, curr->numGChild, curr->data);
		curr = curr->next;
	}
}


long long childSum(struct Node* head, int childNum) {

	struct Node* curr = head;
	long long childsSum = 0;
	while(curr != NULL) {
		if(curr->numGChild == childNum)
			childsSum += curr->data;


		curr = curr->next;
	}

	return childsSum;
}


void findSum(struct Node* head) {
	struct Node* curr = head->next;
	sum = 0;
	while(curr != NULL) {
		sum += curr->data;
		curr = curr->next;
	}
}

void run_part3(void* num, void* numThreads, void* numGcThreads) {
	printf("\n\n\n\nParent: Start of PART3 Execution\n") ;
	currChild = 0;
	currGChild = 0;
	runningPart3 = 1;
	//this for loop is for printing and debugging purposes moves sumList head pointer up to disregard results from run_part()
	for(long int i = 0; i < atol(numThreads) && sumList != NULL; i++) {
		struct Node* temp = sumList;
		sumList = sumList->next;
		free(temp);
	}

	unsigned long int range = (atol(num) / atol(numThreads));
	unsigned long int orgNum = atol(num);
	int numGChilds = atoi(numGcThreads);

	unsigned long int curNum = 0;

	pthread_t tid[atol(numThreads)]; // the thread identifier
	pthread_attr_t attr[atol(numThreads)]; // set of thread attributes

	for(long long i = 0; i < atol(numThreads); i++) {
		tid[i] = (pthread_t)NULL;
	}

	unsigned long int startIndex = 1;
	unsigned long int endIndex = range;
	char* strt; char* strt2;
	char* end;
	for(long int i = 0; i < atoi(numThreads) && startIndex <= orgNum && endIndex <= orgNum; i++) {

		pthread_attr_init(&attr[i]); // set the default attributes of thread


		unsigned long int length = snprintf( NULL, 0, "%d", startIndex );
		strt = malloc( length + 1 );
		snprintf( strt, length + 1, "%d", startIndex );

		length = snprintf( NULL, 0, "%d", endIndex );
		end = malloc( length + 1 );
		snprintf( end, length + 1, "%d", endIndex );

		length = snprintf( NULL, 0, "%d", numGChilds );
		char* granChildren = malloc( length + 1 );
		snprintf( granChildren, length + 1, "%d", numGChilds );

		length = snprintf( NULL, 0, "%llu", range );
		char* ChildIntervalSize = malloc( length + 1 );
		snprintf( ChildIntervalSize, length + 1, "%d", range );

		//puts(ChildIntervalSize);
		//strcpy(strt2,(char*) strt);
		strcat(strt, ",");
		strcat(strt, end);

		strcat(strt, ",");
		strcat(strt, granChildren);

		strcat(strt, ",");
		strcat(strt, ChildIntervalSize);


		length = snprintf( NULL, 0, "%d", i );
		char* temp = malloc( length + 1 );
		snprintf( temp, length + 1, "%d", i );
		strcat(strt, ",");
		strcat(strt, temp);


		startIndex = endIndex + 1;
		endIndex += range;

		//printf("run_part3 passes '%s' to runner2 for new child thd \n", strt);


		pthread_create(&tid[i], &attr[i], runner2, (void *)strt); //creates the thread //strt = startIndex,EndIndex,NumGC, ChildIntervalSize, ChildNum

		//printf("!!Child thread number %d: \t child start=%llu \t end = %llu \t  childsum = %llu \n ", pthread_self(), strt, end, 66);


	}

	for(long int i = 0; i < atol(numThreads) /*&& startIndex <= orgNum && endIndex <= orgNum*/; i++) {

		pthread_join(tid[i], NULL);
		//printf("!!Child thread number %d: \t child start=%s \t end = %s \t  childsum = %llu \n ", pthread_self(), strt, end, 66);
	}
	//findSum(sumList /*, &sum*/);
	//printf("sum FOUND %d ", pthread_self(), sum);


	findSum(sumList /*, &sum*/);
	printf("Parent: Total sum is %ld \n", sum);
	printf("Parent: End of  Execution\n");
	runningPart3 = 0;
}

void *runner2(void *param) {

	//printf("childNUM: %s\n", param);
	//printf("Runner2 takes in '%s' from run_part3 new thread \n", param);

	char* curr = param;
	//strcpy(curr, (char*)param);
	char* tok = strtok(curr,",");
	int cnt = 0; long int start = 0; long int end = 0; long int numGC = 0; long int ChildIntervalSize = 0;
	char* endString; char* numGCString; char* startString; char* ChildIntervalSizeString; char* childNumString;

	while(tok != NULL) {
		if(cnt == 0) {
			startString = tok;
			start = atoi(tok);
		}
		else if(cnt == 1) {
			endString = tok;
			end = atoi(tok);
		}
		else if(cnt == 2) {
			numGC = atol(tok);
			numGCString = tok;
		}
		else if(cnt == 3) {
			ChildIntervalSize = atol(tok);
			ChildIntervalSizeString = tok;

		}
		else if(cnt == 4) {
			childNumString = tok;
		}
		cnt++;

		tok = strtok(NULL,","); // tokenizing string to be used as (void* param) for runner function
	}

	long int gChildIntervalSize = ChildIntervalSize / numGC;
	//int chSum = sumInterval(sumList, currChild++, numGC);
	//printf("Child thread number %d: \t child start=%llu \t end = %llu \t  childsum = %llu \n ", pthread_self(), start, end, 66);
	//printList(sumList);
	//printf("PASS TO GRANDCHILD THREAD:%llu, %llu, %llu, %llu \n", start, end, numGC, gChildIntervalSize);

	pthread_t tid[numGC]; // the thread identifier
	pthread_attr_t attr[numGC]; // set of thread attributes

	for(long long i = 0; i < numGC; i++) {
		tid[i] = (pthread_t)NULL;
	}

	unsigned long int startIndex = start;
	unsigned long int endIndex = (start + gChildIntervalSize) -1 ;
	for(long int i = 0; i < numGC /*&& startIndex <= orgNum && endIndex <= orgNum*/; i++) {

		pthread_attr_init(&attr[i]);


		unsigned long int length = snprintf( NULL, 0, "%d", startIndex );
		char* strt = malloc( length + 1 );
		snprintf( strt, length + 1, "%d", startIndex );
		//puts(strt);

		length = snprintf( NULL, 0, "%d", endIndex );
		char* end = malloc( length + 1 );
		snprintf( end, length + 1, "%d", endIndex );
		//puts(end);

		//char* param = "";
		strcat(strt, ",");
		strcat(strt, end);
		//puts(strt);
		// strcat(param, ",");
		strcat(strt, ",");
		strcat(strt, childNumString);

		//printf("String passed to runner = '%s'\n", strt);

		startIndex = endIndex + 1;
		endIndex += gChildIntervalSize;

		pthread_create(&tid[i], &attr[i], runner, (void *)strt); // creates grandchild threads
		//pthread_join(tid[i], NULL); // wait for the thread to exit


	}

	for(long int i = 0; i < numGC /*&& startIndex <= orgNum && endIndex <= orgNum*/; i++) {
		pthread_join(tid[i], NULL);
	}
	long long currChildSum = childSum(sumList, atoi(childNumString));
	printf("Child thread number %s: \t child start=%llu \t end = %llu \t  childsum = %llu \n ", childNumString, start, end, currChildSum);

	pthread_exit(0);
}



void run_part2(void* num, void* numThreads) {
	printf("\nParent: Start of PART2 Execution\n") ;
	currChild = 0;
	currGChild = 0;
	//sumArr = new int[numThreads];

	unsigned long int range = (atol(num) / atol(numThreads));
	//unsigned long int range = (atol(tempEnd) / atol(numThreads));
	unsigned long int orgNum = atol(num);

	//	printf("\n*Starting Integer: %s\n", num);
	//	printf("*Number of Workers/Threads: %s\n", numThreads);
	//	printf("*Interval Size: %d\n\n", range);


	//	printf("*originalNum: %d\n", orgNum);


	unsigned long int curNum = 0;

	pthread_t tid[atol(numThreads)]; // the thread identifier
	pthread_attr_t attr[atol(numThreads)]; // set of thread attributes

	for(long long i = 0; i < atol(numThreads); i++) {
		tid[i] = (pthread_t)NULL;
	}


	unsigned long int startIndex = 1;
	unsigned long int endIndex = range;
	for(long int i = 0; i < atoi(numThreads) && startIndex <= orgNum && endIndex <= orgNum; i++) {

		pthread_attr_init(&attr[i]); // set the default attributes of thread

		unsigned long int length = snprintf( NULL, 0, "%d", startIndex );
		char* strt = malloc( length + 1 );
		snprintf( strt, length + 1, "%d", startIndex );
		//puts(strt);

		length = snprintf( NULL, 0, "%d", endIndex );
		char* end = malloc( length + 1 );
		snprintf( end, length + 1, "%d", endIndex );
		//puts(end);

		strcat(strt, ",");
		strcat(strt, end);
		//puts(strt);

		startIndex = endIndex + 1;
		endIndex += range;

		//	pthread_create(&tid[i], &attr[i], runner, num); // creates the thread //ex: num = 50,start,end,index
		pthread_create(&tid[i], &attr[i], runner, (void *)strt); // creates the thread //ex: num = 50,start,end,index
	}

	for(long int i = 0; i < atoi(numThreads) /*&& startIndex <= orgNum && endIndex <= orgNum*/; i++) {

		pthread_join(tid[i], NULL);
	}
	findSum(sumList);
	printf("Parent: Total sum is %ld \n", sum);
	printf("Parent: End of  Execution\n");

}


int main(int arc, char** argv) {
	runningPart3 = 0;
	//printf("\nParent: Start of PART2 Execution\n") ;
	sumList = (struct Node*) malloc(sizeof(struct Node));
	sumList->data = -1; sumList->next = NULL;
	//tail = sumList;


	run_part2(argv[1], argv[2]);


	run_part3(argv[1], argv[2], argv[3]);

	deleteList(sumList);
	return 0;
}





void *runner(void *param) {

	//printf("\nIN RUNNER, PARAM = %s\n", param);
	char* tok = strtok(param,",");
	unsigned long int argNum = 0;
	unsigned long int startIndx = -1;
	unsigned long int endIndx = -1;
	char* childNumString;
	int childNumInt = -1;
	char* startIndxString;
	while(tok != NULL) {
		if(argNum == 0) {
			startIndx = atoi(tok);

		}
		else if(argNum == 1) {
			endIndx = atoi(tok);
		}
		else if(argNum == 2) {
			childNumString = tok;
			childNumInt = atoi(tok);
		}

		argNum++;
		//printf("-%s\n", tok);
		tok = strtok(NULL,","); // tokenizing string to be used as (void* param) for runner function
	}

	//printf("\n\nStart: %u\nEnd: %u\n\n", startIndx, endIndx);


	unsigned long int i, upper = atol(param);
	//printf("upper = %s \n", (char *)param);
	//printf("upper = %u \n", upper);

	long long tempSum = 0;

	for(i = startIndx; i <= endIndx; i++) {
		tempSum += i;
	}

	add(sumList, tempSum, childNumInt);
	if(runningPart3 == 1)
		printf("\tChild thread number %s, GrandChild thread number %d: \t grandchild start=%ld \t grandchildend = %ld \t grandchildsum = %ld \n", childNumString , currGChild++, startIndx, endIndx, tempSum);
	else
		printf("\tChild thread number %d: \t child start=%ld \t end = %ld \t sum = %ld \n", currChild++, startIndx, endIndx, tempSum);

	pthread_exit(0);

	return NULL;
}

