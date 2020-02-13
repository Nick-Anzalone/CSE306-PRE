#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *input[200][200]; //a 2-d string array storing the input csv file

int main(int argc, const char* argv[]){
	char filename[500];
	memset(filename, '\0', 500);
	strcpy(filename, argv[argc-1]);
	
	FILE *fp;
	if((fp = fopen(filename, "r")) == NULL){
		printf("invalid file name\n");
		exit(1);
	}



	for(int i=1; i<argc; i++){
		if(strcmp("-r", argv[i]) == 0){

		}else if(strcmp("-h", argv[i]) == 0){
			
		}else if(strcmp("-f", argv[i]) == 0){
		
		}else if(strcmp("-max", argv[i]) == 0){
		
		}else if(strcmp("-min", argv[i]) == 0){
		
		}else if(strcmp("-mean", argv[i]) == 0){
		
		}else if(strcmp("-records", argv[i]) == 0){
		
		}
	}

	return 0;
}

