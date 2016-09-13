%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

graph: graph.cmyMath.o charLines.o graph.o
	$(CC) -lm myMath.o charLines.o graph.o

graph2: myMath.o charLines.o graph2.o
	$(CC) -lm myMath.o charLines.o graph2.o
