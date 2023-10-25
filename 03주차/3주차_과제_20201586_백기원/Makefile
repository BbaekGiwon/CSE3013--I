cc=g++
cflags=-W -g

target=exefile
objects=main.o Str.o

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects)

$(objects) : Str.h

.PHONY:clean
clean:
	rm $(target) $(objects)
