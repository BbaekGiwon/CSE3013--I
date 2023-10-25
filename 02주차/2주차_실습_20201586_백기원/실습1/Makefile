cc=gcc
cflags= -W -g
target=exefile

objects=main.o merge.o mergesort.o

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects)


$(objects) : header.h

.PHONY : clean
clean:
	rm $(target) $(objects)
