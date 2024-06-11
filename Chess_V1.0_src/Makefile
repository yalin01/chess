# Variable
CC = gcc
DEBUG = -g
CFLAGS = -Wall -std=c11 $(DEBUG) -c
LFLAGS = -Wall -std=c11 $(DEBUG)

GTKINC	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --cflags gtk+-2.0`
GTKLIBS	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --libs gtk+-2.0`

INC	= $(GTKINC)
LIBS	= $(GTKLIBS)
#TEST = -DTEST_MODE

#default target
all: chess test_rulecheck test_boarddisplay chess_gui
test: test_rulecheck test_boarddisplay chess
	./bin/chess
clean:
	rm -f bin/chess
	rm -f bin/test_rulecheck
	rm -f bin/test_boarddisplay
	rm -f bin/chess_gui
	rm -f src/*.o
	rm -f bin/log.txt


#specific target
rules.o: src/rules.c src/rules.h src/struct.h
	$(CC) $(CFLAGS) src/rules.c -o src/rules.o

chess_gui.o: src/chess_gui.c src/chess_gui.h src/struct.h src/rules.h
	$(CC) $(CFLAGS) $(INC) src/chess_gui.c -o src/chess_gui.o



chess.o: src/chess.c src/rules.h src/struct.h
	$(CC) $(CFLAGS) src/chess.c -o src/chess.o
test_rulecheck.o: src/test_rulecheck.c src/rules.h src/struct.h
	$(CC) $(CFLAGS) src/test_rulecheck.c -o src/test_rulecheck.o
test_boarddisplay.o: src/test_boarddisplay.c src/rules.h src/struct.h
	$(CC) $(CFLAGS) src/test_boarddisplay.c -o src/test_boarddisplay.o



chess: chess.o rules.o
	$(CC) $(LFLAGS)  src/chess.o src/rules.o -o bin/chess

chess_gui: chess_gui.o rules.o
	$(CC) $(LFLAGS) $(LIBS) src/chess_gui.o src/rules.o -o bin/chess_gui


test_rulecheck: test_rulecheck.o rules.o
	$(CC) $(LFLAGS)  src/test_rulecheck.o src/rules.o -o bin/test_rulecheck

test_boarddisplay: test_boarddisplay.o rules.o
	$(CC) $(LFLAGS)  src/test_boarddisplay.o src/rules.o -o bin/test_boarddisplay

