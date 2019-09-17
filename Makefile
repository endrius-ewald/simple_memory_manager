#Para escrever comentários ##
#Definimos a variável
##CC=gcc

all:	clean install

clean:
	@echo "Make clean"
	rm -rf *.o

install: memoria.c
	@echo "Make install"
	gcc memoria.c -o memoria.o 

run:
	@echo "Make run"
	./memoria.o
