CC      = cc
CFLAGS  = -I./include/
LDFLAGS = -L./lib/ -I./include/ -l:libraylib.a -lm

mycraft: game.o core.o state.o blocks.o
	$(CC) -o mycraft game.o core.o state.o blocks.o src/main.c $(CFLAGS) $(LDFLAGS)

game.o: src/game.c
	$(CC) -c src/game.c $(CFLAGS)

core.o: src/core.c
	$(CC) -c src/core.c $(CFLAGS)

state.o: src/state.c
	$(CC) -c src/state.c $(CFLAGS)

blocks.o: src/blocks.c
	$(CC) -c src/blocks.c $(CFLAGS)

clean:
	rm -f *.o mycraft

.PHONY: clean
