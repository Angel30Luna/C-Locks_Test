rwmain: main.o
	gcc -Wall -lpthread -o rwmain main.o
main.o: main.c
	gcc -Wall -lpthread -c main.c
clean: 
	rm rwmain main.o
