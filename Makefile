vcs: main.o vcs.o diff.o diffoutput.o patch.o
	cc main.o vcs.o diff.o diffoutput.o patch.o -lm -o vcs
main.o: main.c vcs.h
	cc -c main.c
vcs.o: vcs.c vcs.h
	cc -c vcs.c
diff.o: diff.c diff.h
	cc -c diff.c
diffoutput.o: diffoutput.c diffoutput.h
	cc -c diffoutput.c
patch.o: patch.c patch.h
	cc -c patch.c