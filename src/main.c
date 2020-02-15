#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../include/stringList.h"
#include <float.h>
#include <errno.h>

/* Read the data from the input csv file into the buffer declared below */
/* Parameter: fp: pointer to the input file                             */
void read_into_buffer(FILE *fp);


/* Display the maximum value in the indicated field        */
/* Parameter: field: string containing the indicated field */
void calc_max(const char *field);


/* Display the minimum value in the indicated field        */
/* Parameter: field: string containing the indicated field */
void calc_min(const char *field);


/* Display the arithmetic mean value in the indicated field */
/* Parameter: field: string containing the indicated field  */
void calc_mean(const char *field);


/* Display the records from file containing value in the indicated field */
/* Parameters: field: string containing the indicated field              */
/*             value: string containing the value being looked for       */
void find_records(const char *field, const char *value);


/* Initialize some of the global variables */
void init();


/* print the final results */
void print_results();

/* A 2-d array storing the data from the input csv file       */
/* use buffer[i][j] to access the data at ith row, jth column */
char ***buffer;

//stores the number of rows in the input csv file
int rows;

//stores the number of columns in the input csv file
int columns;

// indicates if the user wants to treat the first record as a header record //
// value: 0: do not treat the first record as a header record               //
//        1: treat the first record as a header record                      //
int h;

// r/f will be set to 1 once -r/-f flags are received. Used for //
// preventing printing multiple -r -f results                   //
int r;
int f;

//Stores the fields for -max flags in case the user wants to //
//use -max multiple times in a single command                //
stringList *maxFields;

//stores the fields for -min flags
stringList *minFields;

//stores the fields for -mean flags
stringList *meanFields;

//stores the fields for -record flags
stringList *recordFields;

//stores the values for -record flags
stringList *recordValues;

//stores user input commands
int commandList[20];

//stores the current index for adding new command to commandList
int listIndex;

#define CALCMAX 0
#define CALCMIN 1
#define CALCMEAN 2
#define FINDRECORDS 3
#define CALCFIELDS 4
#define CALCRECORDS 5

int main(int argc, const char* argv[]){

	if(argc < 3){
		printf("invalid arguments\n");
		exit(1);
	}
	//get the path to the file from user input
	char filename[500];
	memset(filename, '\0', 500);
	strcpy(filename, argv[argc-1]);

	//open the file
	FILE *fp;
	if((fp = fopen(filename, "r")) == NULL){
		printf("invalid file name\n");
		exit(1);
	}

	init();

	//count number of rows and columns
	char c;
	bool inside_quote = false;
	bool first_line = true;
	while((c = fgetc(fp)) != EOF){
		if(c == '"'){
			inside_quote = !inside_quote;
		}
		if(c == ',' && !inside_quote && first_line){
			columns++;
		}
		if(c == '\n' && !inside_quote){
			rows++;
			if(first_line){
				columns++;
				first_line = false;
			}
		}
	}
	rewind(fp);

	//dynamically allocate memory for the 2-d buffer array
	buffer = malloc(rows*sizeof(char*));
	for(int i=0; i<rows; i++){
		buffer[i] = malloc(columns*sizeof(char*));
		for(int j=0; j<columns; j++){
			buffer[i][j] = malloc(1000);
			memset(buffer[i][j], '\0', 1000);
		}
	}
	//function for reading data into the 2-d array
	read_into_buffer(fp);

	//processing command line arguments
	for(int i=1; i<argc-1; i++){
		if(strcmp("-max", argv[i]) == 0){
			if(maxFields == NULL){
				maxFields = malloc(sizeof(stringList));
				maxFields->data = malloc(strlen(argv[i+1])+1);
				strcpy(maxFields->data, argv[i+1]);
				maxFields->next = NULL;
			}else{
				SL_add(maxFields, argv[i+1]);
			}
			commandList[listIndex] = CALCMAX;
			listIndex++;
			i++;

		}else if(strcmp("-min", argv[i]) == 0){
			if(minFields == NULL){
				minFields = malloc(sizeof(stringList));
				minFields->data = malloc(strlen(argv[i+1])+1);
				strcpy(minFields->data, argv[i+1]);
				minFields->next = NULL;
			}else{
				SL_add(minFields, argv[i+1]);
			}
			commandList[listIndex] = CALCMIN;
			listIndex++;
			i++;

		}else if(strcmp("-mean", argv[i]) == 0){
			if(meanFields == NULL){
				meanFields = malloc(sizeof(stringList));
				meanFields->data = malloc(strlen(argv[i+1])+1);
				strcpy(meanFields->data, argv[i+1]);
				meanFields->next = NULL;
			}else{
				SL_add(meanFields, argv[i+1]);
			}
			commandList[listIndex] = CALCMEAN;
			listIndex++;
			i++;

		}else if(strcmp("-records", argv[i]) == 0){
			if(recordFields == NULL){
				recordFields = malloc(sizeof(stringList));
				recordFields->data = malloc(strlen(argv[i+1])+1);
				strcpy(recordFields->data, argv[i+1]);
				recordFields->next = NULL;
			}else{
				SL_add(recordFields, argv[i+1]);
			}
			if(recordValues == NULL){
				recordValues = malloc(sizeof(stringList));
				recordValues->data = malloc(strlen(argv[i+2])+1);
				strcpy(recordValues->data, argv[i+2]);
				recordValues->next = NULL;
			}else{
				SL_add(recordValues, argv[i+2]);
			}
			commandList[listIndex] = FINDRECORDS;
			listIndex++;
			i+=2;
		}else if(argv[i][0] == '-'){
			for(int j=1; argv[i][j] != '\0'; j++){
				if(argv[i][j] == 'r'){
					if(r == 0){
					commandList[listIndex] = CALCRECORDS;
					r = 1;
					listIndex++;
					}
				}else if(argv[i][j] == 'f'){
					if(f == 0){
					commandList[listIndex] = CALCFIELDS;
					f = 1;
					listIndex++;
					}
				}else if(argv[i][j] == 'h'){
					h = 1;
				}else{
					printf("Invalid Arguments\n");
					exit(1);
				}
			}
		}
	}

	//print the results
	print_results();

	return 0;
}

void init(){

	//initialization
	maxFields = NULL;
	minFields = NULL;
	meanFields = NULL;
	recordFields = NULL;
	recordValues = NULL;

	rows = 0;
	columns = 0;
	h = 0;
	r = 0;
	f = 0;
	listIndex = 0;
}

void read_into_buffer(FILE *fp){
	int current_row = 0;
	int current_col = 0;

	char c;
	bool inside_quote = false;

	while((c = fgetc(fp)) != EOF){
		if(c == '"'){
			inside_quote = !inside_quote;
			continue;
		}
		if(c == ',' && !inside_quote){
			current_col++;
			continue;
		}
		if(c == '\n' && !inside_quote){
			current_row++;
			current_col = 0;
			continue;
		}

		/* 	Since the lines terminate with \r\n, we want to prevent \r
			from being recorded into the values in the last column.
		*/
		if (c != '\r') {

			strncat(buffer[current_row][current_col], &c, 1);
		}
	}
}

void print_results(){
	for(int i=0; i<listIndex; i++){
		int command = commandList[i];
		if(command == CALCMAX){
			calc_max(maxFields->data);
			maxFields = maxFields->next;
		}
		else if(command == CALCMIN){
			calc_min(minFields->data);
			minFields = minFields->next;
		}
		else if(command == CALCMEAN){
			calc_mean(meanFields->data);
			meanFields = meanFields->next;
		}
		else if(command == CALCFIELDS){
			printf("%d\n", columns);
		}
		else if(command == CALCRECORDS){
			if (h == 1) {
				// Treat the first line as a header, therefore subtract 1 from the data records.
				printf("%d\n", (rows-1));
			} else {
				printf("%d\n", rows);
			}
		}
		else if(command == FINDRECORDS){
			find_records(recordFields->data, recordValues->data);
			recordFields = recordFields->next;
			recordValues = recordValues->next;
		}
	}
}

void calc_max(const char *field){
	int colOfField = -1;
	int startrow = h ? 1 : 0;
	double max = DBL_MIN;
	bool has_numeric = false;

	/*for loop goes through each collumn in first row to find specified field*/
	for(int i = 0; i<columns; i++){
		int comp = strcmp(buffer[0][i],field);
		if(comp == 0){
			colOfField = i;
		}
	}
	if(colOfField == -1){
		printf("Field %s does not exist.\n", field);
		exit(1);
	}
	/*once field is found goes through every value in that field to find max */
	for(int i = startrow; i<rows; i++){
		double temp;
		char *end;

		//Set errno to 0 prior to strol call.
		errno = 0;

		temp = strtod(buffer[i][colOfField], &end);
		if(end == buffer[i][colOfField] || (errno != 0 && temp == 0)){
			continue;
		}
		has_numeric = true;
		if(temp>max){
			max = temp;
		}
	}
	if(has_numeric == false){
		printf("Field %s does not have numeric value in any record.\n", field);
		exit(1);
	}
	printf("%.2f\n", max);
}

void calc_min(const char *field){
	int colOfField = -1;
 	double min =DBL_MAX;
 	bool has_numeric = false;
	/*for loop goes through each collumn in first row to find specified field*/
 	for(int i = 0; i<columns; i++){
		int comp = strcmp(buffer[0][i],field);
		if(comp == 0){
			colOfField = i;
		}
	}
	if(colOfField == -1){
		printf("Field %s does not exist.\n", field);
		exit(1);
	}
	/*once field is found goes through every value in that field to find min */
	for(int i = 0; i<rows; i++){

		//Bugfix to make sure non-number values don't get read.
		char *readNumStr = buffer[i][colOfField];
		char *endptr = NULL;

		//Set errno to 0 prior to strol call.
		errno = 0;

		//Prior to bugfix: int test = atoi(buffer[i][colOfField]);
		double test = strtod(readNumStr, &endptr);

		if (readNumStr == endptr) {
			// Do nothing.
			//printf("Found invalid cell where no nums were found (v1) %s", readNumStr);
		} else if (errno != 0 && test == 0) {
			// Do nothing.
			//printf("Found invalid cell where no nums were found (v2) %s", readNumStr);
		} else {
			has_numeric = true;
			if(test<min) {
				min = test;
			}
		}
	}
	if (has_numeric == false){
		printf("Field %s does not have numeric value in any record.\n", field);
		exit(1);
	}
	printf("%.2f\n",min);
}

void calc_mean(const char *field){

	int colOfField = -1;
 	double sum = 0;
 	int n = 0;
	bool has_numeric = false;

	/*for loop goes through each collumn in first row to find specified field*/
 	for(int i = 0; i<columns; i++){
		int comp = strcmp(buffer[0][i],field);
		if(comp == 0){
			colOfField = i;
		}
	}
	if(colOfField == -1){
		printf("Field %s does not exist.\n", field);
		exit(1);
	}
	/*once field is found goes through every value in that field to find min */
	for(int i = 0; i<rows; i++){

		char *readNumStr = buffer[i][colOfField];
		char *endptr = NULL;

		//Set errno to 0 prior to strol call.
		errno = 0;

		double test = strtod(readNumStr, &endptr);

		if (readNumStr == endptr) {
			// Do nothing.
			//printf("Found invalid cell where no nums were found (v1) %s", readNumStr);
		} else if (errno != 0 && test == 0) {
			// Do nothing.
			//printf("Found invalid cell where no nums were found (v2) %s", readNumStr);
		} else {
			has_numeric = true;
			sum += test;
			n++;
		}
	}

	if (has_numeric == false){
		printf("Field %s does not have numeric value in any record.\n", field);
		exit(1);
	}

	float average = sum / (float) n;
	// printf("sum %d\n", sum);
	// printf("n %d\n", n);
	printf("%.2f\n",average);
}

void find_records(const char *field, const char *value){
	bool hasVal = false;
	int colOfField = -1;
	for(int i = 0; i<columns; i++){
		int comp = strcmp(buffer[0][i],field);
		if(comp == 0){
			colOfField = i;
		}
	}
	if(colOfField == -1){
		printf("Field %s does not exist.\n", field);
		exit(1);
	}

	for(int i = 0; i<rows; i++){
		int comp = strcmp(value, buffer[i][colOfField]);
		if(comp == 0){
			for(int j = 0; j<columns; j++){
				if(j == columns-1){
					printf("%s",buffer[i][j]);
					hasVal = true;
				}else{


				printf("%s,",buffer[i][j]);
				hasVal = true;
			}
			}
			printf("\n");
		}

	}

	if(hasVal == false){
		printf("There are 0 recods with that Value");
	}
}
