CC = clang++
BINARIES = bin
SOURCE = src

default:
	$(CC) $(SOURCE)/*.cpp -o $(BINARIES)/main -Wall --std=c++11