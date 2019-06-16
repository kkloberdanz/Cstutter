CFLAGS=-std=iso9899:1990
WARN_FLAGS=-Wall -Wextra -Wpedantic -Werror
CLANG=clang -Wassign-enum -Wenum-conversion
GCC=gcc
SANITIZE=-fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined

OBJS=lexer parser minic main linkedlist ir assembler growstring linkedlist \
	 bst stackmachine instructions

release: OPTIM_FLAGS=-Os
release: all

debug: OPTIM_FLAGS=-Og -ggdb -DDEBUG $(SANITIZE)
debug: all

CC=$(GCC) $(OPTIM_FLAGS) $(CFLAGS) $(WARN_FLAGS)

all: $(OBJS)
	$(CC) -o minic \
             main.o \
			 minic.o \
			 linkedlist.o \
			 instructions.o \
			 ir.o \
			 lex.yy.o \
			 y.tab.o -lfl -ly

main:
	$(CC) -c main.c

stackmachine: instructions
	$(CC) -c stackmachine.c
	$(CC) -o stackmachine stackmachine.o instructions.o

bst:
	$(CC) -c bst.c

assembler: linkedlist bst instructions
	$(CC) -c assembler.c
	$(CC) -o minias \
		     assembler.o \
			 linkedlist.o \
			 bst.o \
			 instructions.o

instructions:
	$(CC) -c instructions.c

minic:
	$(CC) -c minic.c

growstring:
	$(CC) -c growstring.c

linkedlist:
	$(CC) -c linkedlist.c

ir:
	$(CC) -c ir.c

lexer: parser
	lex tokens.l
	$(CC) -c lex.yy.c -Wno-unused-function

parser:
	yacc -y -d grammar.y
	$(CC) -c y.tab.c

lint: clean
	splint *.c

test: build_ll_test build_gs_test build_bst_test
	rm -f testreport.log
	echo "Test results" >> testreport.log
	date >> testreport.log

	echo "Testing: gs_test" >> testreport.log && \
		valgrind ./gs_test 2>> testreport.log

	echo "Testing: ll_test" >> testreport.log && \
		valgrind ./ll_test 2>> testreport.log

	echo "Testing: bst_test" >> testreport.log && \
		valgrind ./bst_test 2>> testreport.log

	less testreport.log

build_bst_test:
	rm -f bst_test
	$(CC) -o bst_test bst.c tests/bst_test.c

build_ll_test:
	rm -f ll_test
	$(CC) -o ll_test linkedlist.c tests/ll_test.c

build_gs_test:
	rm -f gs_test
	$(CC) -o gs_test growstring.c tests/gs_test.c

clean:
	rm -f *.o
	rm -f minic
	rm -f lex.yy.c
	rm -f y.tab.c
	rm -f y.tab.h
	rm -f testreport.log
	rm -f *_test
	rm -f stackmachine
	rm -f minias
