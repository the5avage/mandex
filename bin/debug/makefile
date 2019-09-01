prog = mandex

CFLAGS += -Wall -Wextra -pedantic-errors
OPT = -g -O0 -fsanitize=address -fno-omit-frame-pointer
LDLIBS = `sdl2-config --cflags --libs` -lasan
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

