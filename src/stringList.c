#include <stdlib.h>
#include <string.h>
#include "../include/stringList.h"

void SL_add(stringList *list, const char *data){
	stringList *ptr = list;
	while(ptr->next!=NULL){
		ptr = ptr->next;
	}

	stringList *new = malloc(sizeof(stringList));
	int dataSize = strlen(data);
	new->data = malloc(dataSize+1);
	strcpy(new->data, data);
	new->next = NULL;

	ptr->next = new;
}
