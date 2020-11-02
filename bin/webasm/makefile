VPATH = ../../src

prog = mandex.js

CC = emcc

CFLAGS = -O3 -Wall

LDLIBS = -s WASM=1 -s USE_SDL=2

LDFLAGS = -Wall -s ALLOW_MEMORY_GROWTH=1

src = $(wildcard $(VPATH)/*.c)
obj_tmp = $(src:.c=.o)
obj = $(patsubst ../../src/%,%,$(obj_tmp))

all: $(prog)

$(prog): $(obj)
	$(CC) $(LDFLAGS) $(LDLIBS) $(obj) -o $(prog)

run: $(prog)
	emrun mandex.html

.PHONY: clean
clean:
	$(RM) $(obj) $(prog)

