var = -lm -lSDL2 -lSDL2_image -lSDL2_ttf
prog : etape3.o fonctions.o
		gcc etape3.o fonctions.o -o prog $(sdl2-config --cflags --libs) $(var)

etape3.o : etape3.c structs.h
		gcc -c etape3.c $(sdl2-config --cflags --libs) $(var)

fonctions.o : fonctions.c structs.h
		gcc -c fonctions.c $(sdl2-config --cflags --libs) $(var)

clean :
		rm -f *.o