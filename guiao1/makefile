all: main

main: main.o deque.o node.o
	gcc main.o deque.o node.o -o main

main.o: main.c
	gcc -c main.c -o main.o

deque.o: deque.c 
	gcc -c deque.c -o deque.o

node.o: node.c
	gcc -c node.c -o node.o

clean:
	rm node.o deque.o main.o main