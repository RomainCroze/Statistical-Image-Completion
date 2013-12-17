
CC=g++
CFLAGS=-Wall
LDFLAGS=
EXEC=algo
LIBS=-lX11 -ljpeg -lpthread


all: $(EXEC)

algo: Include/GCO/graph.o Include/GCO/LinkedBlockList.o Include/GCO/maxflow.o Include/GCO/GCoptimization.o GUI.o patchmatch.o occurrences.o switch.o graphcut.o main.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

main.o: Include/header.h

Include/GCO/GCoptimization.o: Include/GCO/GCoptimization.h Include/GCO/LinkedBlockList.h

Include/GCO/LinkedBlockList.o: Include/GCO/LinkedBlockList.h

Include/GCO/graph.o: Include/GCO/graph.h

Include/GCO/maxflow.o: Include/GCO/graph.h

GUI.o: Include/GUI.h

patchmatch.o: Include/patchmatch.h

occurrences.o: Include/occurrences.h

switch.o: Include/switch.h

graphcut.o: Include/graphcut.h

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
