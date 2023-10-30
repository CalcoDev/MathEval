CC = clang -g -O0
CCFLAGS = -Wall -Wextra -Wpedantic -Wstrict-aliasing -Wno-newline-eof \
	-Wno-gnu-zero-variadic-macro-arguments -Wno-unused-parameter
LDFLAGS = ""

SRC = $(wildcard src/*.c src/*/*.c src/*/*/*.c)
OBJ = $(patsubst src/%, bin/obj/%, $(SRC:.c=.o))

.PHONY: dirs clean

run: all
	bin/math_eval

all: dirs math_eval

math_eval: $(OBJ)
	$(CC) -o bin/math_eval $^ $(LDFLAGS)

bin/obj/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -c $^ -o $@

dirs:
	echo "$(OBJ)"
	echo "$(SRC)"
	mkdir -p bin
	mkdir -p bin/obj

clean:
	rm -rf bin/*