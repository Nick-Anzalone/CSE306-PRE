#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

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



/* A 2-d array storing the data from the input csv file       */
/* use buffer[i][j] to access the data at ith row, jth column */
char ***buffer;

//stores the number of rows in the input csv file
int rows = 0;

//stores the number of columns in the input csv file
int columns = 0;

// indicates if the user wants to treat the first record as a header record //
// value: 0: do not treat the first record as a header record               //
//        1: treat the first record as a header record                      //
int h = 0;



int main(int argc, const char* argv[]){
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
		if(strcmp("-r", argv[i]) == 0){
			printf("%d\n", rows);
			continue;

		}else if(strcmp("-h", argv[i]) == 0){
			h = 1;
			continue;

		}else if(strcmp("-f", argv[i]) == 0){
			printf("%d\n", columns);
			continue;

		}else if(strcmp("-max", argv[i]) == 0){
			calc_max(argv[i+1]);
			i++;
		
		}else if(strcmp("-min", argv[i]) == 0){
			calc_min(argv[i+1]);
			i++;
		
		}else if(strcmp("-mean", argv[i]) == 0){
			calc_mean(argv[i+1]);
			i++;
		
		}else if(strcmp("-records", argv[i]) == 0){
			find_records(argv[i+1], argv[i+2]);
			i+=2;
		}
	}

	return 0;
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
		strncat(buffer[current_row][current_col], &c, 1);
	}
}

void calc_max(const char *field){
	//printf("print the result here\n");
}

void calc_min(const char *field){
	//printf("print the result here\n");
}

void calc_mean(const char *field){
	//printf("print the result here\n");
}

void find_records(const char *field, const char *value){
	//printf("print the result here\n");
}
