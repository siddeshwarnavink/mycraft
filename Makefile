CC      = cc
CFLAGS  = -I./include/
LDFLAGS = -L./lib/ -I./include/ -l:libraylib.a -lm
OBJS = game.o core.o state.o blocks.o sounds.o inventory.o textures.o

mycraft: $(OBJS)
	$(CC) -o mycraft $(OBJS) src/main.c $(CFLAGS) $(LDFLAGS)

game.o: src/game.c
	$(CC) -c src/game.c $(CFLAGS)

core.o: src/core.c
	$(CC) -c src/core.c $(CFLAGS)

state.o: src/state.c
	$(CC) -c src/state.c $(CFLAGS)

blocks.o: src/blocks.c
	$(CC) -c src/blocks.c $(CFLAGS)

sounds.o: src/sounds.c
	$(CC) -c src/sounds.c $(CFLAGS)

inventory.o: src/inventory.c
	$(CC) -c src/inventory.c $(CFLAGS)

textures.o: src/textures.c
	$(CC) -c src/textures.c $(CFLAGS)

clean:
	rm -f *.o mycraft

.PHONY: clean
