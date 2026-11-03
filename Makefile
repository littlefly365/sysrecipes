PROG=lpkg

SRCS=lpkg.c
OBJS=$(SRCS:.c=.o)

CPPFLAGS ?=
CFLAGS ?=
LDFLAGS ?=

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $? -o $@ $(LDFLAGS)
%.o: %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@
clean:
	rm -rf $(PROG) $(OBJS)
.PHONY: all clean
