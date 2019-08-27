prog = mandex
debug = mandex_dbg

CFLAGS += -Wall -Wextra -pedantic-errors
OPT = -O3 -march=native
DBG = -g -O0 -fsanitize=address -fno-omit-frame-pointer
LDBG = -lasan

src = $(wildcard *.c)
obj = $(src:.c=.o)

LDLIBS = `sdl2-config --cflags --libs`

all: $(prog)

dbg: $(debug)

$(debug): CFLAGS += $(DBG)
$(debug): LDFLAGS += $(DBG)
$(debug): LDLIBS += $(LDBG)
$(debug): $(obj)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(prog): CFLAGS += $(OPT)
$(prog): LDFLAGS += $(OPT)
$(prog): $(obj)

.PHONY: clean
clean:
	rm -f $(obj) $(prog) $(debug)
