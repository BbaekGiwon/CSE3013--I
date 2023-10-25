cc=g++
cflags=-W -g
target=exefile

objects=main.o array.o rangearray.o

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects)

main.o: main.cpp Array.h RangeArray.h
	$(cc) $(cflags) -c main.cpp

array.o: array.cpp Array.h
	$(cc) $(cflags) -c array.cpp

rangearray.o: rangearray.cpp RangeArray.h
	$(cc) $(cflags) -c rangearray.cpp

.PHONY: clean
clean:
	rm $(target) $(objects)
