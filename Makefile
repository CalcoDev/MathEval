CC = clang -g -O0
CCFLAGS = -Wall -Wextra -Wpedantic -Wstrict-aliasing -Wno-newline-eof \
	-Wno-gnu-zero-variadic-macro-arguments -Wno-unused-parameter
LDFLAGS = 

# ifeq ($(OS),Windows_NT)
# 	LDFLAGS += -lmsvcrt.lib -lshell32.lib -lUser32.lib -lKernel32.lib \
# 		-lGdi32.lib -lAdvapi32.lib -lComctl32.lib -lOle32.lib -lShLwApi.lib \
# 		-lUserEnv.lib -lWindowsApp.lib
# endif

SRC = $(wildcard src/*.c src/*/*.c src/*/*/*.c)
OBJ = $(patsubst src/%, bin/obj/%, $(SRC:.c=.o))

.PHONY: dirs assets clean

run: all
	bin/math_eval

all: dirs math_eval assets

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

assets:
	mkdir -p bin/assets;

	rm -rf bin/assets;
	cp -r assets/. bin/assets;

clean:
	rm -rf bin/*