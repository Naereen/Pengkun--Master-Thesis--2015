# Minimalist Makefile for a C++ project
all: run

playGame:
	g++ -std=c++11 -o playGame *.cpp algorithm/*.cpp

run: playGame
	./playGame | tee -a ./playGame.log
