ALL_OBJS = tools.o hashtable.o lexer.o init.o parser.o
main: main.o $(ALL_OBJS) compiler.o
	$(CC) -Wall -o main main.o $(ALL_OBJS) compiler.o
test: test.o $(ALL_OBJS)
	$(CC) -Wall -o test test.o $(ALL_OBJS)
preprocessor: preprocessor.c hashtable.o tools.o
	$(CC) -Wall -o preprocessor tools.o hashtable.o preprocessor.c
%.o: %.c %.h misc.h tools.h
	$(CC) -Wall -c -o $@ $<
%.out: %
prog.out: main tests/progs/*.in tests/progs/*.v tests/progs/*.out 
	tests/final.sh
%.out: test tests/%/*.in tests/%/*.out
	tests/script.sh $*
list:
	@ tests/list.sh
clean:
	rm -f *.o
clean_test:
	rm -f *.out
