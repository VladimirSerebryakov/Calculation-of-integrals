CFLAGS ?= -O2 -g

CFLAGS += -std=gnu99

CFLAGS += -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
	-Wswitch-default -Wpointer-arith -Wtype-limits -Wempty-body \
	-Wstrict-prototypes -Wold-style-declaration -Wold-style-definition \
	-Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs \
	-Wstack-usage=4096 -Wmissing-prototypes -Wfloat-equal

CFLAGS += -fsanitize=undefined -fsanitize-undefined-trap-on-error

CC += -m32 -no-pie -fno-pie

LDLIBS = -lm

.PHONY: all clean

all: integral

integral: integral.o asm_func.o test_func.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

integral.o: integral.c
	$(CC) $(CFLAGS) -c -o $@ $<

asm_func.o: asm_func.asm
	nasm -f elf32 -o $@ $<

test_func.o: test_func.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f integral *.o

test:
	rm -f integral *.o
	$(MAKE) integral APP_FLAGS="-DTEST_MODE"
	./integral --test-root 4:5:0.0:2.5:1e-5:2.0
	./integral --test-root 4:6:-1.0:1.0:1e-5:0.0
	./integral --test-root 5:6:-2.0:0.0:1e-5:-1.146193
	./integral --test-integral 4:0.0:2.0:1e-5:2.0
	./integral --test-integral 5:0.0:3.0:1e-5:3.0
	./integral --test-integral 6:0.0:1.0:1e-5:0.718282

