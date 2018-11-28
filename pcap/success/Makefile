objects = main.o goose.o
 
pcaptest : $(objects)
	gcc -o pcaptest  $(objects)
 
main.o:goose.h 
goose.o:goose.h

.PHONY : clean
clean :
	rm pcaptest  $(objects)
