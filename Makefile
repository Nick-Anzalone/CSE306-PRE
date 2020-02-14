csv:./src/main.c ./src/stringList.c ./include/stringList.h
	gcc -g -Wall -o csv ./src/main.c ./src/stringList.c

clean:
	rm ./csv
