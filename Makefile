CC = g++

CFLAGS = -Wall

SRC = quickrun.cpp

EXE = bin/qrun

$(EXE): $(SRC)
	$(CC) $(CFLAGS) -o $(EXE) $(SRC)

clean:
	rm -f $(EXE)

.PHONY: all clean
