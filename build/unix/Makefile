INCLUDES = -I "../../src"
SRC = ../../src/*.c	../../src/platform/unix/*.c
CFLAGS = -std=c89

rnbench: $(SRC)
	$(CC) -o $@ $(SRC) -O2 $(CFLAGS) $(INCLUDES)

rnbench-dbg: $(SRC)
	$(CC) -o $@ $(SRC) -g $(CFLAGS) $(INCLUDES)

clean:
	rm -rf *.o rnbench rnbench-dbg rnbench-dbg.dSYM	
