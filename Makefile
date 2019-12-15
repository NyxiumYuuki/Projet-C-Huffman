CC = gcc
CCOPTS = -Wall

all: main_compress

arbre_binaire.o:	arbre_de_codage/arbre_binaire.c arbre_de_codage/arbre_binaire.h
	${CC} ${CCOPTS} -c arbre_de_codage/arbre_binaire.c

gestion_fichiers.o:	gestion_des_fichiers/gestion_fichiers.c gestion_des_fichiers/gestion_fichiers.h
	${CC} ${CCOPTS} -c gestion_des_fichiers/gestion_fichiers.c

main_compress.o:	main_compress.c
	${CC} ${CCOPTS} -c main_compress.c

main_compress:	main_compress.o arbre_binaire.o gestion_fichiers.o
	${CC} ${CCOPTS} -o main_compress main_compress.o arbre_binaire.o gestion_fichiers.o

clean:
	rm *.o ; rm main_compress
