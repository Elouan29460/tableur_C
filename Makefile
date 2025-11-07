prog : testListe.o liste.o
	gcc -o prog testListe.o liste.o -lm

testListe.o : testListe.c
	gcc -c testListe.c

liste.o : liste.c
	gcc -c liste.c
