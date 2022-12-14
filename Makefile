##---------------------------------------
## Target file to be compiled by default
##---------------------------------------

MAIN=main

##---------------------------------------
## CC is the compiler to be used
##---------------------------------------

CC=gcc

##---------------------------------------
## CFLAGS are the options passed to the compiler
##---------------------------------------

CFLAGS= -Wall -std=c11 -lm

##---------------------------------------
## SRC relative path of source files
##---------------------------------------

SRC= src

EXT= extern/kissfft

##---------------------------------------
## OBJS are the object files to be linked
##---------------------------------------

#OBJ2= mylib2
#OBJS= $(MAIN).o $(OBJ1).o $(OBJ2).o

OBJ1= graphics
OBJ2= kiss_fft

OBJS= $(MAIN).o $(OBJ1).o $(OBJ2).o

##---------------------------------------
## LIBS are the external libraries to be used
##---------------------------------------

LIBS=`allegro-config --libs`

##---------------------------------------
##DEPENDENCIES
##---------------------------------------
$(MAIN): $(OBJS)
	$(CC) -o $(MAIN) $(OBJS) $(LIBS) $(CFLAGS)

$(MAIN).o: $(SRC)/$(MAIN).c
	$(CC) -c $(SRC)/$(MAIN).c

$(OBJ1).o: $(SRC)/$(OBJ1).c
	$(CC) -c $(SRC)/$(OBJ1).c

$(OBJ2).o: $(SRC)/$(EXT)/$(OBJ2).c
	$(CC) -c $(SRC)/$(EXT)/$(OBJ2).c

clean:
	@rm -f $(MAIN) *.o
