CC = clang++
BINARIES = bin
SOURCE = src

default:
	$(CC) $(SOURCE)/*.cpp -o $(BINARIES)/main