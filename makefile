
all: clean compile link

link:
	g++ -o bin/studio bin/main.o bin/Action.o bin/Customer.o bin/Studio.o bin/Trainer.o bin/Workout.o

compile:
	g++ -g -Wall -std=c++11 -c -o bin/Workout.o src/Workout.cpp
	g++ -g -Wall -std=c++11 -c -o bin/Customer.o src/Customer.cpp
	g++ -g -Wall -std=c++11 -c -o bin/Action.o src/Action.cpp
	g++ -g -Wall -std=c++11 -c -o bin/Trainer.o src/Trainer.cpp
	g++ -g -Wall -std=c++11 -c -o bin/Studio.o src/Studio.cpp
	g++ -g -Wall -std=c++11 -c -o bin/main.o src/main.cpp

clean:
	rm -f bin/*
