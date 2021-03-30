diff: main.o diff.o diffoutput.o
	cc main.o diff.o diffoutput.o -lm -o diff
main.o: main.c diff.h
	cc -c main.c
diff.o: diff.c diff.h
	cc -c diff.c
diffoutput.o: diffoutput.c diffoutput.h
	cc -c diffoutput.c