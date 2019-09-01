prog = mandex

CFLAGS += -Wall -Wextra -pedantic-errors
OPT = -O3 -march=native
LDLIBS = `sdl2-config --cflags --libs`
VPATH = ../../src

src = $(wildcard $(VPATH)/*.c)
obj_tmp = $(src:.c=.o)
obj = $(patsubst ../../src/%,%,$(obj_tmp))

all: $(prog)

$(prog): CFLAGS += $(OPT)
$(prog): LDFLAGS += $(OPT)
$(prog): $(obj)

run: $(prog)
	./$(prog)

.PHONY: clean
clean:
	$(RM) $(obj) $(prog)

