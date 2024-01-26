CFLAGS= -Wall -Wextra -O2 -g -Iinclude `pkg-config --cflags glib-2.0`
CC = gcc
LDFLAGS = `pkg-config --libs glib-2.0` -lcurses -lm

all:programa-principal programa-testes

programa-principal: src/main.o src/date.o src/flight.o src/interactive.o src/interpreter.o src/manageFlights.o src/managePassengers.o src/manageReservations.o src/manageUsers.o src/output.o src/output_interactive.o src/parser.o src/passenger.o src/queries.o src/reservation.o src/statistics.o src/user.o src/utility.o src/validation.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

programa-testes: src/main_tests.o src/date.o src/flight.o src/interpreter_tests.o src/manageFlights.o src/managePassengers.o src/manageReservations.o src/manageUsers.o src/output.o src/parser.o src/passenger.o src/queries.o src/reservation.o src/statistics.o src/user.o src/output_test.o src/output_interactive.o src/utility.o src/validation.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

src/main.o: src/main.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/main_tests.o: src/main_tests.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/interactive.o: src/interactive.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/interpreter.o: src/interpreter.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/interpreter_tests.o: src/interpreter_tests.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/output_test.o: src/output_test.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/queries.o: src/queries.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/parser.o: src/parser.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/manageReservations.o: src/manageReservations.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/manageFlights.o: src/manageFlights.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/managePassengers.o: src/managePassengers.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/manageUsers.o: src/manageUsers.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/flight.o: src/flight.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/reservation.o: src/reservation.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/user.o: src/user.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/passenger.o: src/passenger.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/statistics.o: src/statistics.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/validation.o: src/validation.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/output.o: src/output.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/output_interactive.o: src/output_interactive.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/utility.o: src/utility.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/date.o: src/date.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f programa-principal programa-testes src/*.o