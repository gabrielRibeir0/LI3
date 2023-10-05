all: main

main: main.o command_parser.o helper_functions.o ../guiao1/deque.o ../guiao1/node.o
	gcc main.o command_parser.o helper_functions.o ../guiao1/deque.o ../guiao1/node.o -o main

main.o: main.c
	gcc -c main.c -o main.o

command_parser.o: command_parser.c
	gcc -c command_parser.c -o command_parser.o

helper_functions.o: helper_functions.c
	gcc -c helper_functions.c -o helper_functions.o

../guiao1/deque.o: ../guiao1/deque.c
	gcc -c ../guiao1/deque.c -o ../guiao1/deque.o

../guiao1/node.o: ../guiao1/node.c
	gcc -c ../guiao1/node.c -o ../guiao1/node.o

clean:
	rm *.o main
	rm ../guiao1/deque.o ../guiao1/node.o