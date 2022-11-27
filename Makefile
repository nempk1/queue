CC := gcc
CFLAGS := -ggdb3 -Wall
LDFLAGS := -lpthread
TARGETS := queue.o test

queue.o: queue.c queue.h
	$(CC) -c $< $(CFLAGS) -o $@

test: test.c queue.o
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

.PHONY : clean
clean :
	rm -rf $(TARGETS)
