include ../../makefile.variable

VPATH = ../../src

prog = $(program_name)

CFLAGS += $(compiler_flags)

LDLIBS += $(libraries)

LDFLAGS += $(release_flags)
CFLAGS += $(release_flags)


src = $(wildcard $(VPATH)/*.c)
obj_tmp = $(src:.c=.o)
obj = $(patsubst ../../src/%,%,$(obj_tmp))

all: $(prog)

$(prog): $(obj)

run: $(prog)
	./$(prog)

.PHONY: clean
clean:
	$(RM) $(obj) $(prog)

