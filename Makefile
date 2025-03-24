CC       = cc
CFLAGS   = -I./include/
LDFLAGS  = -L./lib/ -I./include/ -l:libraylib.a -lm
SRCS     = $(wildcard src/*.c)
BUILDDIR = .build/linux/
OBJS     = $(patsubst src/%.c, $(BUILDDIR)%.o, $(SRCS))

mycraft: $(BUILDDIR) $(OBJS)
	$(CC) -o $(BUILDDIR)mycraft $(OBJS) $(CFLAGS) $(LDFLAGS)
	ln -sf resources $(BUILDDIR)resources

$(BUILDDIR)%.o: src/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)

.PHONY: clean
